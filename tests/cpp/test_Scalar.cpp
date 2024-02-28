#include <vfs/vfs_system/VirtualCtStringList.h>
#include <vfs/Scalar.h>
#include "catch_main.h"

// TEST_CASE( "Scalar", "" ) {
//     Scalar a = CtInt<10>{};
//     P( a );
// }

// TEST_CASE( "Scalar from pointer", "" ) {
//     int a = 10;
//     Scalar s( FromPointer(), &a );
//     P( s );

//     //     Scalar t = s;
//     //     P( t );

//     //     // Scalar a( CtInt<10>{} );
//     //     // Scalar b( a );
//     //     // P( b );
// }

TEST_CASE( "Scalar from pointer", "" ) {
    VFS_CALL( P, CtStringList<>, void, VirtualCtStringList( { "yo", "ya" } ) );
}
