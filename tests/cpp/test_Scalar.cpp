#include <vfs/Scalar.h>
#include "catch_main.h"

TEST_CASE( "Scalar", "" ) {
    Scalar a( 10 );
    P( a );

    // Scalar a( CtInt<10>{} );
    // Scalar b( a );
    // P( b );
}
