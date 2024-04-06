#include "catch_main.h"
#include <vfs/Type.h>

TEST_CASE( "Type", "" ) {
    Type t = CtType<PI32>();
    P( t );
    // template<class Item,class Memory,int static_size,int alignment,int local_size,bool allow_heap>
    // class alignas( std::max( PI( alignment ), alignof( Item ) ) ) GenericVector {

    // Type u( "GenericVector", "vfs/support/containers/GenericVector.h", CtType<PI32>(), Type( "Memory_Cpu", "vfs/support/containers/Memory/Memory_Cpu.h" ), 3, CtInt<0>(), CtInt<0>(), CtInt<0>() );
    // P( u );
}
