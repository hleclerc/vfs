#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

bool size_and_lexical_comparison( const auto &a, const auto &b ) {
    if ( a.size() != b.size() )
        return a.size() < b.size();
    for( PI i = 0; i < a.size(); ++i )
        if ( a[ i ] != b[ i ] )
            return a[ i ] < b[ i ];
    return false;
}

END_VFS_NAMESPACE
