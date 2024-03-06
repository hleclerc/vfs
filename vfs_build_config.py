def config( options ):
    options.add_cpp_flag( '-std=c++20' )
    options.add_inc_path( 'src/cpp' )
    
    options.add_cpp_flag( '-fPIC' )
    options.add_lib_flag( '-fPIC' )
    

