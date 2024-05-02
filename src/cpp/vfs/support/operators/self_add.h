#pragma once

#include "default_operators.h"
#include "../IsAlwaysZero.h"

BEG_VFS_NAMESPACE

struct Functor_self_add {
    static auto type_name() { return "VFS_NAMESPACE::Functor_self_add"; }
    auto operator()( auto &a, auto &&b ) const { return self_add( a, FORWARD( b ) ); }
};

constexpr auto self_add( auto &a, auto &&b ) {
    // ... + 0
    if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( b )>::value ) {
        return PrimitiveCtInt<1>();
    } else

    // default behavior
    DEFAULT_BIN_SELF_OPERATOR_CODE_SIGN( self_add, + )

    // // Str concatenation
    // if constexpr( requires{ StrView( a ); StrView( b ); } ) {
    //     std::string_view sa( a );
    //     std::string_view sb( b );

    //     std::string res;
    //     res.resize( sa.size() + sb.size() );
    //     std::copy( sa.begin(), sa.end(), res.data() );
    //     std::copy( sb.begin(), sb.end(), res.data() + sa.size() );
    //     return res;
    // } else

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call self add" );
    return PrimitiveCtInt<0>();
}

END_VFS_NAMESPACE
