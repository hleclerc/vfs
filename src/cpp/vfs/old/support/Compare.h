#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE
// declarations
Tij constexpr SI compare_tuple( const auto &a, const auto &b, CtInt<i>, CtInt<j> );

/// We have our own compare because <=> may be considered as a vector operation
SI compare( const auto &a, const auto &b ) {
    // as method
    if constexpr ( requires { a.compare( b ); } ) {
        return a.compare( b );
    } else

    if constexpr ( requires { b.compare( a ); } ) {
        return - b.compare( a );
    } else

    // string like
    if constexpr ( StrLike<decltype(a)> && StrLike<decltype(b)> ) {
        return StrView( a ).compare( StrView( b ) );
    } else

    // vector like
    if constexpr ( requires { a.size(); b.size(); a[ 0 ]; b[ 0 ]; } ) {
        if ( SI d = a.size() - b.size() )
            return d;
        for( PI i = 0; i < a.size(); ++i )
            if ( SI d = compare( a[ i ], b[ i ] ) )
                return d;
        return 0;
    } else

    // tuple like
    if constexpr ( requires { std::tuple_size<std::decay_t<decltype(a)>>{}; std::tuple_size<std::decay_t<decltype(b)>>{}; } ) {
        constexpr PI sa = std::tuple_size<std::decay_t<decltype(a)>>{};
        constexpr PI sb = std::tuple_size<std::decay_t<decltype(b)>>{};
        if constexpr ( constexpr SI d = sa - sb )
            return d;
        else
            return compare_tuple( a, b, CtInt<0>(), CtInt<sa>() );
    } else

    // by default (BEWARE: does not work for float, ...)
    return SI( a ) - SI( b );
}

Tij constexpr SI compare_tuple( const auto &a, const auto &b, CtInt<i>, CtInt<j> ) {
    if constexpr ( i < j ) {
        if ( SI d = compare( std::get<i>( a ), std::get<i>( b ) ) )
            return d;
        return compare_tuple( a, b, CtInt<i+1>(), CtInt<j>() );
    } else {
        return 0;
    }
}



/// Operator that can be used for std::map, std::set, ...
struct Less {
    constexpr SI operator()( const auto &a, const auto &b ) const {
        return compare( a, b ) < 0;
    }
};

/// helper to compare sets of pairs of values
inline SI compare_chain() {
    return 0;
}

SI compare_chain( const auto &a, const auto &b, const auto &...tail ) {
    if ( SI d = compare( a, b ) )
        return d;
    return compare_chain( tail... );
}

END_VFS_NAMESPACE
