#include <vfs/vfs_system/VirtualArgList.h>
#include <vfs/Scalar.h>
#include "catch_main.h"

TEST_CASE( "Scalar", "" ) {
    Scalar b = 17;
    P( b );
}

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

// TEST_CASE( "Scalar from arg list", "" ) {
//     VirtualArgList val;
//     val.add_owned( new int( 17 ) );
//     val.add_owned( new int( 18 ) );
//     VFS_CALL( P, CtStringList<>, void, std::move( val ) );
// }
