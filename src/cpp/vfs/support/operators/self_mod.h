#pragma once

#include "default_operators.h"
// #include "../IsAlwaysZero.h"

BEG_VFS_NAMESPACE

struct Functor_self_mod {
    static auto template_type_name() { return "VFS_NAMESPACE::Functor_self_mod"; }
    void operator()( auto &a, auto &&b ) const { a %= FORWARD( b ); }
};

constexpr void self_mod( auto &a, auto &&b ) {
    // default behavior
    DEFAULT_BIN_SELF_OPERATOR_CODE_SIGN( self_mod, %=, % )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call self_mod" );
}

END_VFS_NAMESPACE
