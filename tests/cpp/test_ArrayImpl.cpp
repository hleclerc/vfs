#include <vfs/containers/VecImpl.h>
#include <vfs/support/type_name.h>
#include "catch_main.h"

TEST_CASE( "ArrayImpl", "" ) {
    VecImpl<double,2> v( FromItemValues(), 1, 2 );
    P( 100 + v );
}
