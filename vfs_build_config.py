def config( options ):
    options.add_cpp_flag( '-std=c++20' )
    options.add_cpp_flag( '-fpic' )
    options.add_inc_path( 'src/cpp' )
    

