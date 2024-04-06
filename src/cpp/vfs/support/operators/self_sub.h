#pragma once

#include "default_operators.h"
#include "../IsAlwaysZero.h"

BEG_VFS_NAMESPACE

constexpr void self_sub( auto &a, auto &&b ) {
    // ... - 0
    if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( b )>::value ) {
        return;
    } else

    // default behavior
    DEFAULT_BIN_SELF_OPERATOR_CODE_SIGN( self_sub, -=, - )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call self sub" );
}

END_VFS_NAMESPACE
