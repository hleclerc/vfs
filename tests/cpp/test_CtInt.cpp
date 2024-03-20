#include <vfs/support/CtInt.h>
#include "catch_main.h"

TEST_CASE( "CtInt", "" ) {
    CtInt<12> a;
    P( a + a );
    // P( a + 10 );
}
