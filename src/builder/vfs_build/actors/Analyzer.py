from ..support.Options import Options 
from .Command import Command 
from .Actor import Actor 

from pathlib import Path
import tempfile
import shlex
import os

class Analyzer( Actor ):
    def on_start( self, source: str ) -> None:
        self.add_source_dep( source )

        self.source = source    
        self.tmp_file = tempfile.NamedTemporaryFile()
        self.launch( self.on_gcc_analysis, Command(), self.flags() )

    def on_gcc_analysis( self ):
        # read the content
        content = shlex.split( self.tmp_file.read().decode() )
        self.tmp_file.close()

        # we store the flags that may be changed after reading a new vfs_build_config.py
        old_flags = self.flags()

        # parse it
        includes = []
        for path in content:
            path = path.strip()
            if path == '' or path.endswith( ":" ):
                continue
            includes.append( os.path.abspath( path ) )
            self.add_source_dep( path )

        # relaunch if flags have changed
        new_flags = self.flags()
        if new_flags != old_flags:
            if self.options.verbosity() >= 2:
                self.info( "re-analysis after new flag set" )
            self.tmp_file = tempfile.NamedTemporaryFile()
            return self.launch( self.on_gcc_analysis, Command(), self.flags() )

        # else, we're done
        self.on_end( includes )

    def flags( self ):
        return compile_flags( 
            self.options.compiler_for( Path( self.source ).suffix ), 
            self.tmp_file.name, 
            self.source, 
            self.options, 
            for_analysis = True
        )

def compile_flags( compiler, out: str, src: str, options: Options, for_analysis = False ):
    res = [ compiler, src ]
    if for_analysis:
        res += [ '-MM', '-MF', out ]
    else:
        res += [ '-o', out, '-c' ]
    for option in options.all_the_options():
        if option[ 0 ] == 'inc-path':
            res.append( f'-I{ option[ 1 ] }' )
            continue
        if option[ 0 ] == 'cpp-flag':
            res.append( str( option[ 1 ] ) )
            continue
        if option[ 0 ] == 'define':
            res.append( f'-D{ option[ 1 ] }' )
            continue
    return res
