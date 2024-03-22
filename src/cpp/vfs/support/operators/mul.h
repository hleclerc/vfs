#pragma once

#include "../IsAlwaysZero.h"
#include "default_operators.h"

BEG_VFS_NAMESPACE

struct Functor_mul {
    auto operator()( auto &&a, auto &&b ) const { return FORWARD( a ) * FORWARD( b ); }
};

constexpr auto mul( auto &&a, auto &&b ) {
    // 0 * b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( a );
    } else

    // a * 0
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( b ) ) ) {
        return FORWARD( b );
    } else

    DEFAULT_BIN_OPERATOR_CODE_SIGN( mul, * )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call mul" );
}

END_VFS_NAMESPACE
