#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

/// seq argmin
std::size_t argmax( auto &&a ) requires ( tensor_order( CT_DECAYED_TYPE_OF( a ) ).always_equal( 1 ) ) {
    auto res = 0;
    for( std::size_t i = 1; i < a.size(); ++i )
        if ( a[ res ] < a[ i ] )
            res = i;
    return res;
}

END_VFS_NAMESPACE
