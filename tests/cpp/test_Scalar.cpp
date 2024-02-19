#include <vfs/Scalar.h>
#include "catch_main.h"

TEST_CASE( "Scalar", "" ) {
    Scalar a( 10 );
    P( a );

    // Scalar a( CtInt<10>{} );
    // Scalar b( a );
    // P( b );
}

// TEST_CASE( "Scalar from pointer", "" ) {
//     int a = 10;
//     Scalar s( FromPointer(), &a );
//     P( s );

//     Scalar t = s;
//     P( t );

//     // Scalar a( CtInt<10>{} );
//     // Scalar b( a );
//     // P( b );
// }
