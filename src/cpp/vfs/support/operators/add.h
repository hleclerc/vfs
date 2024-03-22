#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

struct Functor_add {
    auto operator()( auto &&a, auto &&b ) const { return FORWARD( a ) + FORWARD( b ); }
};

constexpr auto add( auto &&a, auto &&b ) {
    // 0 + ..., ... + 0
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( b );
    } else

    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( b ) ) ) {
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
