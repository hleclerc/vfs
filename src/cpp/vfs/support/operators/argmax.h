#pragma once

#include "../TensorOrder.h"
#include "../TypeConfig.h"

BEG_VFS_NAMESPACE

/// seq argmin
std::size_t argmax( auto &&a ) requires ( TensorOrder<DECAYED_TYPE_OF( a )>::value == 1 ) {
    auto res = 0;
    for( PI i = 1; i < a.size(); ++i )
        if ( a[ res ] < a[ i ] )
            res = i;
    return res;
}

END_VFS_NAMESPACE
