#pragma once

#include "PrimitiveCtIntList.h"
#include "IsScalar.h"

BEG_VFS_NAMESPACE

///
template<class T>
struct StaticSizesOf;

// for scalars
template<IsScalar T>
struct StaticSizesOf<T> {
    using value = PrimitiveCtIntList<>;
};

END_VFS_NAMESPACE
