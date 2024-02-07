import vfs

def config( options ):
    options.load_lib( 'https://github.com/catchorg/Catch2.git', lib_names = [ "Catch2Main", "Catch2" ] )
    vfs.vfs_build_config( options )

