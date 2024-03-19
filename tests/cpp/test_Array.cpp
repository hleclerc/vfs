// #include <vfs/Vector.h>
// #include <vfs/Point.h>
// #include <vfs/List.h>

#include "catch_main.h"

TEST_CASE( "Empty array", "" ) {
    Vector b;
    P( b );
}

// TEST_CASE( "Array", "" ) {
//     // // Point a{ 1, 2, 3 };
//     // // P( a.type() );
//     // // P( a );

//     // // Vector b{ 1, 2, 3 };
//     // // b << 6;
//     // // P( b );

//     // // List<Point> c{ { 1, 2, 3 }, { 4, 5, 6 } };
//     // // P( c.type() );
//     // // P( c );

//     using Vol = Array<Scalar>;

//     Vol vol = Vol::fill( { 3 }, 5 );
//     // P( vol( 0 ) );
//     // // vol( 0 ) = 10;
//     P( vol );

//     // // vol( 1 ) = 1.5;
//     // // P( vol );

//     // // P( type_promote( CtString<"">(), CtType<SI32>(), CtType<FP32>() ) );
// }
