from ..support.Options import Options 
from .WriteFile import WriteFile
from .Compiler import Compiler
from .Command import Command
from .Actor import Actor

from pathlib import Path
import os

class Link( Actor ):
    def on_start( self, link_type: str, sources: list[ str ] ) -> None:
        self.nb_sources_to_compile = 0
        self.has_used_sources = False
        self.link_type = link_type
        self.seen_includes = []
        self.seen_sources = []
        self.objects = []
        
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

        # launch compilation (--do-not-link-deps => we compile and link only the first .cpp file)
        if not self.options[ "do-not-link-deps" ] or len( self.seen_sources ) == 1:
            self.nb_sources_to_compile += 1
            self.launch( self.on_compile, Compiler( self.options_to_remove() ), source )

    def on_compile( self, obj: str, deps: list[ str ] ):
        self.nb_sources_to_compile -= 1
        self.objects.append( obj )

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

    def write_used_sources( self ) -> bool:
        if self.has_used_sources:
            return False
        self.has_used_sources = True

        src_content  = "#include <vfs/support/used_sources.h>\n"
        src_content += "#include <vfs/support/OnInit.h>\n"
        src_content += "\n"
        src_content += "ON_INIT {\n"
        for source in self.seen_sources:
            src_content += f'    VFS_NAMESPACE::used_sources.insert( "{ source }" );\n'
        src_content += "}\n"

        self.launch( self.add_source, WriteFile( self.options_to_remove() ), content = src_content, ext = "cpp", stem = "used_sources" )
        return True

    def check_if_ended( self ):
        if self.nb_sources_to_compile:
            return

        # need to write the used sources ?
        if self.options[ "write-used-sources" ] and not self.options[ "do-not-link-deps" ] and self.write_used_sources():
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
        if self.options.verbosity():
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
        self.on_end(
            out_name = self.output_filename,
            cpp_deps = self.seen_sources[ 1: ],
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

