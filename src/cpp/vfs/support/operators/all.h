#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

/// seq any
bool all( auto &&a ) requires ( tensor_order( CT_DECAYED_TYPE_OF( a ) ).always_equal( 1 ) ) {
    bool res = true;
    for( std::size_t i = 0; i < a.size(); ++i )
        res &= a[ i ];
    return res;
}

END_VFS_NAMESPACE
