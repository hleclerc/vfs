def config( options ):
    options.add_option( "write-used-sources" )

    options.add_lib_flag( '-rdynamic' )
    options.add_cpp_flag( '-fpic' )
    

