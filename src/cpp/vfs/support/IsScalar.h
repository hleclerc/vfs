#pragma once

#include "IsAlwaysZero.h"

BEG_VFS_NAMESPACE

template<class T>
struct IsScalar {
    enum { value = IsAlwaysZero<T>::value };
};

END_VFS_NAMESPACE
