#include <vfs/support/Vec.h>
#include "catch_main.h"

TEST_CASE( "Vec", "" ) {
    Vec<int> a{ 1, 2 };
    // P( a );
    P( a + a );
}
