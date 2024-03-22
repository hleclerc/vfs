#include <vfs/support/Vec.h>
#include "catch_main.h"

TEST_CASE( "Vec", "" ) {
    Vec<int,2> a{ 1, 2 };
    PT( a + 10 );
    P( a + 10 );
}
