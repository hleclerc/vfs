def config( options ):
    # options.add_global_option( 'write-used-sources', unique = True )

    options.add_cpp_flag( '-std=c++20' )
    options.add_cpp_flag( '-fPIC' )

    options.add_inc_path( 'src/cpp' )
    

