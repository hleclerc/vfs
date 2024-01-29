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

END_VFS_NAMESPACE
