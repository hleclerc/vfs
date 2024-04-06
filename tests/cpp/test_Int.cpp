#include "catch_main.h"
#include <vfs/Int.h>

TEST_CASE( "Int", "" ) {
    Int a = 11;
    Int b;
    b *= a;
    P( b );
}
