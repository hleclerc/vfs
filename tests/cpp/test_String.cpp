#include <vfs/String.h>

#include "catch_main.h"

TEST_CASE( "String", "" ) {
    String c = "yo";
    P( Str( c ) );
    // c = c + "fdz";
    // P( c );
}
