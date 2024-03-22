#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

struct Functor_sub {
    auto operator()( auto &&a, auto &&b ) const { return FORWARD( a ) - FORWARD( b ); }
};

constexpr auto sub( auto &&a, auto &&b ) {
    // 0 - b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return - FORWARD( b );
    } else

    // a - 0
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( b ) ) ) {
        return FORWARD( a );
    } else

    DEFAULT_BIN_OPERATOR_CODE_SIGN( sub, - )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call sub" );
}

END_VFS_NAMESPACE
