#pragma once

#include "default_operators.h"
// #include "../IsAlwaysZero.h"

BEG_VFS_NAMESPACE

struct Functor_self_div {
    static auto type_name() { return "VFS_NAMESPACE::Functor_self_div"; }
    auto operator()( auto &a, auto &&b ) const { return self_div( a, FORWARD( b ) ); }
};

constexpr auto self_div( auto &a, auto &&b ) {
    // default behavior
    DEFAULT_BIN_SELF_OPERATOR_CODE_SIGN( self_div, /=, / )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call self div" );
    return PrimitiveCtInt<0>();
}

END_VFS_NAMESPACE
