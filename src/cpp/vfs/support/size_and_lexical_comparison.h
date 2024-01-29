#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE
Tij constexpr int size_and_lexical_comparison_tuple( const auto &a, const auto &b, CtInt<i>, CtInt<j> );

constexpr int size_and_lexical_comparison( const auto &a, const auto &b ) {
    if constexpr ( requires { a.size(); b.size(); a[ 0 ]; b[ 0 ]; } ) {
        if ( a.size() != b.size() )
            return a.size() < b.size();
        for( PI i = 0; i < a.size(); ++i ) {
            if ( size_and_lexical_comparison( a[ i ], b[ i ] ) )
                return true;
            if ( size_and_lexical_comparison( b[ i ], a[ i ] ) )
                return false;
        }
        return false;
    } else if constexpr ( requires { std::get<std::tuple_size<std::decay_t<decltype(a)>>{}-1>( a ); std::get<std::tuple_size<std::decay_t<decltype(b)>>{}-1>( b ); } ) {
        constexpr PI sa = std::tuple_size<std::decay_t<decltype(a)>>{};
        constexpr PI sb = std::tuple_size<std::decay_t<decltype(a)>>{};
        if constexpr ( sa != sb )
            return sa < sb;
        else
            return size_and_lexical_comparison_tuple( a, b, CtInt<0>(), CtInt<sa>() );
    } else
        return a < b;
}

Tij constexpr int size_and_lexical_comparison_tuple( const auto &a, const auto &b, CtInt<i>, CtInt<j> ) {
    if constexpr ( i < j ) {
        if ( size_and_lexical_comparison( std::get<i>( a ), std::get<i>( b ) ) )
            return true;
        if ( size_and_lexical_comparison( std::get<i>( b ), std::get<i>( a ) ) )
            return false;
        return size_and_lexical_comparison_tuple( a, b, CtInt<i+1>(), CtInt<j>() );
    } else {
        return false;
    }
}

struct SizeAndLexicalComparison {
    constexpr bool operator()( const auto &a, const auto &b ) const {
        return size_and_lexical_comparison( a, b );
    }
};

END_VFS_NAMESPACE
