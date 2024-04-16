// #include <vfs/Vector.h>
// #include <vfs/Point.h>
#include <vfs/List.h>

#include "catch_main.h"

// TEST_CASE( "Empty array", "" ) {
//     Vector b;
//     P( b );
// }

TEST_CASE( "Rt dim array", "" ) {
    using Vol = List<Int>;
    Vol v;
    v << 17 << 18;
    P( v );

    // Vol v{ 1, 2, 3, 4 };
    // auto t = tuple( Str( "a" ), Str( "b" ), Str( "c" ) );
    // auto r = t.prefix_scan_with_index( []( auto a, auto b, auto i ) { return a + b; }, Str(), CtInt<0>(), CtInt<1>() );
    // P( r );
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
