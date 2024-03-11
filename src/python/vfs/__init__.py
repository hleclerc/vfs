import os

def vfs_build_config( options ):
    """ add to options the flags needed to compile vfs application/libraries """
    options.add_inc_path( os.path.join( __file__, os.path.pardir, os.path.pardir, os.path.pardir, "cpp" ) )

    options.add_global_option( "write-used-sources" )
    options.add_global_option( "write-used-flags" )

    options.add_lib_flag( '-rdynamic' )
    options.add_cpp_flag( '-fpic' )
    

