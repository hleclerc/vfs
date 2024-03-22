#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

struct Functor_neg {
    auto operator()( auto &&a ) const { return - FORWARD( a ); }
};

auto neg( auto &&a ) {
    DEFAULT_UNA_OPERATOR_CODE( neg, - )

    STATIC_ASSERT_IN_IF_CONSTEXPR( a, "found not way to call neg" );
}

END_VFS_NAMESPACE
