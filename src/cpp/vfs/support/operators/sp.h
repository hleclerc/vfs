#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

/// scalar product
auto sp( auto &&a, auto &&b ) {
    auto res = a[ 0 ] * b[ 0 ];
    for( std::size_t i = 1; i < a.size(); ++i )
        res += a[ i ] * b[ i ];
    return res;
}

END_VFS_NAMESPACE
