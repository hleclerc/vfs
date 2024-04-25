#pragma once

#include "StorageTypeFor.h"
#include "CtInt.h"

BEG_VFS_NAMESPACE

template<int beg,int end,int inc> void for_each_value_in_range( auto &&func, CtInt<beg>, CtInt<end>, CtInt<inc> ) {
    if constexpr ( beg < end ) {
        func( beg );
        for_each_value_in_range( FORWARD( func ), CtInt<beg+inc>(), CtInt<end>(), CtInt<inc>() );
    }
}

void for_each_value_in_range( auto &&func, auto &&beg, auto &&end, auto &&inc ) {
    using T = STORAGE_TYPE_OF( beg + inc + end );
    for( T cur = beg; cur < end; cur += inc )
        func( cur );
}

// ------------------------------------------------------------------------------------------------
void for_each_value_in_range( auto &&func, auto &&beg, auto &&end ) {
    for_each_value_in_range( FORWARD( func ), beg, end, CtInt<1>() );
}

void for_each_value_in_range( auto &&func, auto &&end ) {
    for_each_value_in_range( FORWARD( func ), end * CtInt<0>(), end );
}

END_VFS_NAMESPACE

