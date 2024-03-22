#pragma once

#include "default_operators.h"
#include "../IsAlwaysZero.h"

BEG_VFS_NAMESPACE

struct Functor_add {
    auto operator()( auto &&a, auto &&b ) const { return FORWARD( a ) + FORWARD( b ); }
};

constexpr auto add( auto &&a, auto &&b ) {
    // 0 + ..., ... + 0
    if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( a )>::value ) {
        return FORWARD( b );
    } else

    if constexpr( IsAlwaysZero<DECAYED_TYPE_OF( b )>::value ) {
        return FORWARD( a );
    } else

    // Str concatenation
    if constexpr( requires{ StrView( a ); StrView( b ); } ) {
        std::string_view sa( a );
        std::string_view sb( b );

        std::string res;
        res.resize( sa.size() + sb.size() );
        std::copy( sa.begin(), sa.end(), res.data() );
        std::copy( sb.begin(), sb.end(), res.data() + sa.size() );
        return res;
    } else

    // default behavior
    DEFAULT_BIN_OPERATOR_CODE_SIGN( add, + )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call add" );
}

END_VFS_NAMESPACE
