#include <vfs/support/string/string_simple_match.h>
#include <vfs/vfs_system/VirtualArgList.h>
#include <vfs/support/Rational.h>
#include <vfs/Scalar.h>
#include "catch_main.h"

// TEST_CASE( "Scalar", "" ) {
//     Scalar b = 17;
//     P( b );
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

// TEST_CASE( "Scalar from arg list", "" ) {
//     int a = 43;
//     Scalar b = 17;

//     VirtualArgList va;
//     va.add_owned( &a );
//     va.add_owned( &b );
//     VFS_CALL( PT, CtStringList<>, void, std::move( va ) );
// }

TEST_CASE( "Scalar from arg list", "" ) {
    Scalar a = 17;
    Scalar b = 17.6;
    // Scalar c = a / 2;
    P( a.__vfs_dt_attributes.global_type_index );
    P( b.__vfs_dt_attributes.global_type_index );
    P( a.__vfs_dt_attributes.instantiated_type_index );
    P( b.__vfs_dt_attributes.instantiated_type_index );
    P( a );
    P( b );
    P( a.__vfs_dt_attributes.instantiated_type_index );
    P( b.__vfs_dt_attributes.instantiated_type_index );
    // P( c );
    // P( c.__vfs_dt_attributes.instantiated_type_index );
    // P( c.__vfs_dt_attributes.global_type_index );

    // P( a / 2 );

    // P( a.__vfs_dt_attributes.global_type_index );
    // P( b.__vfs_dt_attributes.global_type_index );
    // P( c.__vfs_dt_attributes.global_type_index );
    // P( a );
}
