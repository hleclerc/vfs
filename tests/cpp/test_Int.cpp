#include <vfs/support/CtInt.h>
#include "catch_main.h"
// #include <vfs/Int.h>

// template<class IntType>
// struct VfsWrapedObject {
//     mutable PI32 instantiated_type_index;
//     PI32         global_type_index;
//     IntType*     data;
// };
struct Yo {

};

struct Pouetox : CtInt<10>, Yo {
    // operator CtInt<10>() const { return {}; }
};

template<int v>
void pouet( CtInt<v> ) {
    P( v );
}

TEST_CASE( "Int", "" ) {
    // Int a( FromTypeAndCtorArguments(), CtType<VfsIntegerInstance<SI32>>(), 10 );
    // P( a );
    pouet( Pouetox() );
    // using A = VfsObjectTypeFor<int,int &>::value;
    // PT( A() );
}
