#include <vfs/vfs_system/VirtualArgList.h>
#include <vfs/Function.h>
#include <vfs/Scalar.h>
#include "catch_main.h"

TEST_CASE( "Function", "" ) {
    Function add( "std::max" );

    SI32 yo = 21;

    VirtualArgList ral;
    ral.add_borrowed( &yo );
    ral.add_owned( new Scalar( 20 ) );
    auto res = VFS_CALL( call, CtStringList<>, SI64, add, std::move( ral ) );

    P( res );
}
