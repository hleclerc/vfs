#include <vfs/Vector.h>
#include <vfs/Point.h>
#include <vfs/List.h>

#include "catch_main.h"

TEST_CASE( "Array", "" ) {
    // Point a{ 1, 2, 3 };
    // P( type_of( a ) );
    // P( a );

    // Vector b{ 1, 2, 3 };
    // P( type_of( b ) );
    // P( b );

    List<Point> c{ { 1, 2, 3 }, { 4, 5, 6 } };
    P( type_of( c ) );
    P( c );
}
