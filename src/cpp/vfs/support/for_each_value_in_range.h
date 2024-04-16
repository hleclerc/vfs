#pragma once

#include "StorageTypeFor.h"
#include "CtInt.h"

BEG_VFS_NAMESPACE 

template<int i> void for_each_value_in_range( auto &&func, CtInt<i> ) {
    TODO;
}

void for_each_value_in_range( auto &&func, auto &&end ) {
    using T = StorageTypeFor<decltype(end)>::value;
    for( T v = 0; v < end; ++v )
        func( v );
}

END_VFS_NAMESPACE

