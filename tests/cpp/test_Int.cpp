#include "catch_main.h"
#include <vfs/Int.h>

// template<class IntType>
// struct VfsWrapedObject {
//     mutable PI32 instantiated_type_index;
//     PI32         global_type_index;
//     IntType*     data;
// };

TEST_CASE( "Int", "" ) {
    Int a = 11;
    Int b;

    P( a + 2 );
    // Int b( FromTypeAndCtorArguments(), CtType<VfsImpl_StdInt<SI64>>(), 10 );
    //P( tuple_cat( tuple( 1, 5 ), tuple( "" ) ) );
    // using A = VfsObjectTypeFor<int,int &>::value;
    // PT( A() );
}
