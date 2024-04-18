from ..support.Options import Options 
from .WriteFile import WriteFile
from .Analyzer import Analyzer
from .Compiler import Compiler
from .Command import Command
from .Actor import Actor

from pathlib import Path
import os

class Link( Actor ):
    def on_start( self, link_type: str, sources: list[ str ] ) -> None:
        self.nb_sources_to_compile_or_analyze = 0
        self.link_type = link_type
        self.seen_includes = []
        self.seen_sources = []
        self.objects = []

        self.has_used_sources_deps = False
        self.has_used_flags_deps = False
        self.has_used_sources = False
        self.has_used_flags = False

        for source in sources:
            self.add_source( os.path.abspath( source ) )

        self.check_if_ended()

    def add_source( self, source: str ):
        # sources-to-avoid
        if stas := self.options[ "sources-to-avoid" ]:
            if source in stas.value.split( "," ):
                return

        # register source
        if source in self.seen_sources:
            return 
        self.seen_sources.append( source )

        # get flag + deps
        self.add_source_dep( source )

        # launch compilation (--do-not-link-deps => we compile and link only the first .cpp file)
        if not self.options[ "do-not-link-deps" ] or len( self.seen_sources ) == 1:
            self.nb_sources_to_compile_or_analyze += 1
            self.launch( self.on_compile, Compiler( self.options_to_remove() ), source )

    def on_compile( self, obj: str, deps: list[ str ] ):
        self.objects.append( obj )
        self.on_analysis( deps )

    def on_analysis( self, deps: list[ str ] ):
        self.nb_sources_to_compile_or_analyze -= 1

        for dep in deps:
            if dep.endswith( ".h" ):
                self.add_include( os.path.abspath( dep ) )
                
        self.check_if_ended()

    def add_include( self, include: str ):
        if include in self.seen_includes:
            return 
        self.seen_includes.append( include )
        
        self.add_source_dep( include )

        cpp = str( Path( include ).with_suffix( ".cpp" ) )
        if self.check_file( cpp ):
            self.add_source( cpp )

    def add_deps_for_include( self, includes: list[ str ] ):
        for opt in self.options.all_the_options():
            if opt[ 0 ] == 'inc-path':
                for include in includes:
                    h_name = os.path.join( opt[ 1 ], include )
                    if self.check_file( h_name ):
                        self.nb_sources_to_compile_or_analyze += 1
                        self.launch( self.on_analysis, Analyzer( self.options_to_remove() ), h_name )
                        break

    def get_used_sources_deps( self ):
        self.add_deps_for_include( [ "vfs/support/used_sources.h", "vfs/support/OnInit.h" ] )

    def write_used_sources( self ):
        src_content  = "#include <vfs/support/used_sources.h>\n"
        src_content += "#include <vfs/support/OnInit.h>\n"
        src_content += "\n"
        src_content += "ON_INIT {\n"

        sorted_sources = self.seen_sources.copy()
        sorted_sources.sort()
        for source in sorted_sources:
            src_content += f'    VFS_NAMESPACE::used_sources.insert( "{ source }" );\n'
        src_content += "}\n"

        self.nb_sources_to_compile_or_analyze += 1
        self.launch( self.add_generated_source, WriteFile( self.options_to_remove() ), content = src_content, ext = "cpp", stem = "used_sources" )

    def get_used_flags_deps( self ):
        self.add_deps_for_include( [ "vfs/support/used_flags.h", "vfs/support/OnInit.h" ] )

    def write_used_flags( self ):
        src_content  = "#include <vfs/support/used_flags.h>\n"
        src_content += "#include <vfs/support/OnInit.h>\n"
        src_content += "\n"
        src_content += "ON_INIT {\n"
        for name, value in self.options.all_the_options():
            cvalue = value.replace( '"', '\\\\\\"' )
            src_content += f'    VFS_NAMESPACE::used_flags.push_back( "{ name }", "{ cvalue }" );\n'
        src_content += "}\n"

        self.nb_sources_to_compile_or_analyze += 1
        self.launch( self.add_generated_source, WriteFile( self.options_to_remove() ), content = src_content, ext = "cpp", stem = "used_flags" )

    def add_generated_source( self, source: str ):
        self.launch( self.on_compile, Compiler( self.options_to_remove() ), source )

    def check_if_ended( self ):
        if self.nb_sources_to_compile_or_analyze:
            return

        # first pass of used sources or flags (we look for the dependancies which may change the flags and the sources)
        want_used_flags = self.options[ "write-used-flags" ] and not self.options[ "do-not-link-deps" ]
        if want_used_flags and not self.has_used_flags_deps:
            self.has_used_flags_deps = True
            self.get_used_flags_deps()
            if self.nb_sources_to_compile_or_analyze:
                return
            
        want_used_sources = self.options[ "write-used-sources" ] and not self.options[ "do-not-link-deps" ]
        if want_used_sources and not self.has_used_sources_deps:
            self.has_used_sources_deps = True
            self.get_used_sources_deps()
            if self.nb_sources_to_compile_or_analyze:
                return

        # second pass of used sources or flags (we write the files)
        if want_used_flags and not self.has_used_flags:
            self.has_used_flags = True
            self.write_used_flags()
            if self.nb_sources_to_compile_or_analyze:
                return
            
        if want_used_sources and not self.has_used_sources:
            self.has_used_sources = True
            self.write_used_sources()
            if self.nb_sources_to_compile_or_analyze:
                return

        # extension
        ext = None
        if self.link_type == 'exe':
            ext = "exe"
        elif self.link_type == 'lib':
            from distutils import sysconfig
            ext = sysconfig.get_config_var('SHLIB_SUFFIX')[ 1: ]
        else:
            self.error( f"`{ self.link_type }` is not a known link type" )

        # get self.output_filename
        self.output_filename = self.make_output_filename( sub_dirs = [ 'obj' ], ext = ext, stem = Path( self.seen_sources[ 0 ] ).stem )

        # cmd
        if self.options.verbosity() > 0:
            self.info( f"Link of { self.relative_name( self.output_filename ) } using { self.seen_sources }" )
        else:
            self.info( f"Link of { self.relative_name( self.output_filename ) }" )

        self.launch( self.on_link, Command(), link_flags( 
            self.link_type,
            self.options.linker_for( ["cpp"] ), 
            self.output_filename,
            self.objects,
            self.options
        ) )

    def write_out_info( self, fout, info_type, info_data ):
        assert( info_data.find( "\n" ) < 0 )
        fout.write( info_type + ":" + info_data + "\n" )

    def on_link( self ):
        lib_names = []
        lib_paths = []
        for option in self.options.all_the_options():
            if option[ 0 ] == 'lib-path':
                lib_paths.append( option[ 1 ] )
                continue
            if option[ 0 ] == 'lib-name':
                lib_names.append( option[ 1 ] )
                continue

        self.on_end(
            out_name = self.output_filename,
            cpp_deps = self.seen_sources[ 1: ],
            lib_names = lib_names,
            lib_paths = lib_paths,
        )

    def options_to_remove( self ):
        return [ 
            'lib-path',
            'lib-name',
            'lib-flag',
            'output',
        ]


def link_flags( link_type: str, linker: str, output_path: Path, obj_files: list[ str ] , options: Options ) -> list[str]:
    res = [ linker, '-o', str( output_path ) ]
    for obj_file in obj_files:
        res.append( obj_file )
    if link_type == 'lib':
        res.append( '-shared' )
    for option in options.all_the_options():
        if option[ 0 ] == 'lib-path':
            res.append( f'-L{ option[ 1 ] }' )
            continue
        if option[ 0 ] == 'lib-name':
            res.append( f'-l{ option[ 1 ] }' )
            continue
        if option[ 0 ] == 'lib-flag':
            res.append( str( option[ 1 ] ) )
            continue
    return res

