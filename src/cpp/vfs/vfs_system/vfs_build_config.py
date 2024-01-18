def config( options ):
    options.add_define( f'VFS_SOURCE_DIR="{ options.cwd_parent( 2 ) }"' )
    

