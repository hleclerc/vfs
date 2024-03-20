#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

bool lexical_comparison( const auto &a, const auto &b ) {
    for( PI i = 0; i < std::min( a.size(), b.size() ); ++i )
        if ( a[ i ] != b[ i ] )
            return a[ i ] < b[ i ];
    if ( a.size() != b.size() )
        return a.size() < b.size();
    return false;
}

bool lexical_comparison( const auto &a, const auto &b, auto default_value ) {
    for( PI i = 0; i < std::max( a.size(), b.size() ); ++i ) {
        const auto &va = i < a.size() ? a[ i ] : default_value;
        const auto &vb = i < b.size() ? b[ i ] : default_value;
        if ( va != vb )
            return va < vb;
    }
    return false;
}

END_VFS_NAMESPACE
