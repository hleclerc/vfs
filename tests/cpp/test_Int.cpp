#include "catch_main.h"
#include <vfs/Int.h>

// template<class IntType>
// struct VfsWrapedObject {
//     mutable PI32 instantiated_type_index;
//     PI32         global_type_index;
//     IntType*     data;
// };

TEST_CASE( "Int", "" ) {
    Int a( FromTypeAndCtorArguments(), CtType<VfsImpl_StdInt<SI32>>(), 10 );
    P( a.global_type_index );
    P( a );
    // using A = VfsObjectTypeFor<int,int &>::value;
    // PT( A() );
}
