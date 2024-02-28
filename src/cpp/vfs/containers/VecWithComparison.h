#pragma once

#include "Vec.h"

BEG_VFS_NAMESPACE

template<class T>
class VecWithComparison {
public:
    bool operator<( const VecWithComparison &that ) const {
        return size_and_lexical_comparison( content, that.content );
    }

    Vec<T> content;
};

END_VFS_NAMESPACE
