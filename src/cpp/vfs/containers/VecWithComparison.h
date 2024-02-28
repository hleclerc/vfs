#pragma once

#include "Vec.h"

BEG_VFS_NAMESPACE

template<class T>
class VecWithComparison {
public:
    int operator<( const VecWithComparison &that ) const {
        // return size_and_lexical_comparison( content, that.content ) < 0;
    }

    Vec<T> content;
};

END_VFS_NAMESPACE
