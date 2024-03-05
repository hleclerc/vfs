import subprocess, importlib, sysconfig, shlex, sys, os
sys.setdlopenflags( os.RTLD_GLOBAL | os.RTLD_NOW )

# make cpp module if necessary ------------------------------------------------------------------------
try:
    importlib.import_module( ".dylibs", 'vfs_python_interface' )
except ModuleNotFoundError:
    curdir = os.path.dirname( __file__ )
    srcdir = os.path.dirname( os.path.dirname( os.path.dirname( curdir ) ) )
    dylib = os.path.join( curdir, 'dylibs', 'vfs_python_interface' + sysconfig.get_config_var( 'EXT_SUFFIX' ) )
    source = os.path.join( srcdir, 'cpp', 'vfs', 'python', 'vfs_python_interface.cpp' )
    cp = subprocess.run( [ 'vfs_build', 'lib', '-o' + shlex.quote( dylib ), source ] )
    if cp.returncode:
        sys.exit( cp.returncode )

# import the cpp module -------------------------------------------------------------------------------
from .dylibs.vfs_python_interface import function, add_global_cpp_flag

# pure python -----------------------------------------------------------------------------------------
def method( name, comp_flags = None ):
    return function( name + "__method", comp_flags or [] )

def get_type( name, comp_flags = None ):
    type_func = function( "Type", [ "inc_file:vfs/Type.h" ] )
    return type_func( name, comp_flags or [] )
