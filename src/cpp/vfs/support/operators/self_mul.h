#pragma once

#include "default_operators.h"
// #include "../IsAlwaysZero.h"

BEG_VFS_NAMESPACE

struct Functor_self_mul {
    static auto type_name() { return "VFS_NAMESPACE::Functor_self_mul"; }
    auto operator()( auto &a, auto &&b ) const { return self_mul( a, FORWARD( b ) ); }
};

constexpr auto self_mul( auto &a, auto &&b ) {
    // default behavior
    DEFAULT_BIN_SELF_OPERATOR_CODE_SIGN( self_mul, *=, * )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call self mul" );
    return PrimitiveCtInt<0>();
}

END_VFS_NAMESPACE
