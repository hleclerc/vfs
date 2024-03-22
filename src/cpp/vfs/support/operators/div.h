#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

struct Functor_div {
    auto operator()( auto &&a, auto &&b ) const { return FORWARD( a ) / FORWARD( b ); }
};

constexpr auto div( auto &&a, auto &&b ) {
    // 0 / b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( a );
    } else

    DEFAULT_BIN_OPERATOR_CODE_SIGN( div, / )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call div" );
}

END_VFS_NAMESPACE
