#include "catch_main.h"
#include <vfs/Int.h>

TEST_CASE( "Int", "" ) {
    Int a = 17;
    P( a );
    // using A = VfsObjectTypeFor<int,int &>::value;
    // PT( A() );
}
