#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

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

    DEFAULT_BIN_OPERATOR_CODE( add )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call add" );
}

constexpr auto add( auto &&a, auto &&b );


END_VFS_NAMESPACE
