#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

/// binary
constexpr auto max( auto &&a, auto &&b ) {
    DEFAULT_BIN_OPERATOR_CODE( max )

    return a >= b ? FORWARD( a ) : FORWARD( b );
}

/// seq max
auto max( auto &&a ) requires ( TensorOrder<DECAYED_TYPE_OF( a )>::value == 1 ) {
    using std::max;
    auto res = a[ 0 ];
    for( std::size_t i = 1; i < a.size(); ++i )
        res = max( res, a[ i ] );
    return res;
}

END_VFS_NAMESPACE
