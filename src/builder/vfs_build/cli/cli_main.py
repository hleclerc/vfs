#!/usr/bin/env python

from ..actors.LinkAndLaunch import LinkAndLaunch
from ..actors.Link import Link

from ..support.ActorSystem import ActorSystem
from ..support.Options import Options

import shutil
import sys
import os


def make_sys_exit_0( *args, **kwargs ):
    sys.exit( 0 )


def do_mission( mission: str, sources: list[str], asy: ActorSystem ):
    if mission == 'clean':
        bd = asy.options.build_dir()
        for d in os.listdir( bd ):
            if d == 'ext':
                continue
            shutil.rmtree( os.path.join( bd, d ) )
        sys.exit( 0 )

    if mission == 'run':
        return asy.launch( None, sys.exit, LinkAndLaunch(), sources )

    if mission == 'lib':
        return asy.launch( None, make_sys_exit_0, Link(), 'lib', sources )

    if not mission:
        print( 'Please specify a mission', file = sys.stderr )
        sys.exit( 1 )
        
    print( f'`{ mission }` is not a known mission', file = sys.stderr )
    sys.exit( 2 )


def cli_main():
    # parse args. Simplified format where options are next (no space) to the flags
    # words:
    #   --flag
    #   --flag=...
    # single letter:
    #    -f
    #    -f...
    mission = ''
    sources = []
    options = Options()
    for num_arg in range( 1, len( sys.argv ) ):
        arg = sys.argv[ num_arg ]

        # extra arg
        if arg == '--':
            options.extra_args = sys.argv[ num_arg + 1: ]
            break

        # --flag
        if arg.startswith( '--' ):
            i = arg.find( '=' )
            if i >= 0:
                options.add_option( arg[ 2:i ], arg[ i+1: ] )
            else:
                options.add_option( arg[ 2: ] )
            continue

        # -f
        if arg.startswith( '-' ):
            if len( arg ) == 1:
                print( 'In arguments for vfs_build, `-` must be followed by a character', file=sys.stderr )
                sys.exit( 1 )
            options.add_option( options.single_letter_option_correspondance( arg[ 1 ] ), arg[ 2: ] )
            continue

        # mission
        if mission == '':
            mission = arg
            continue 

        # positionnal arg
        sources.append( arg )

    #
    asy = ActorSystem( options )
    do_mission( mission, sources, asy )
    asy.wait()
