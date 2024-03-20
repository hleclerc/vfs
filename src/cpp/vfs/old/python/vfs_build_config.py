import sysconfig
import numpy

def config( options ):
    options.add_inc_path( sysconfig.get_path( "include" ) )
    options.add_inc_path( numpy.get_include() )
    options.add_inc_path( '../../../cpp' )

    # options.add_lib_flag( '-fpic' )
    options.add_cpp_flag( '-O3' )
