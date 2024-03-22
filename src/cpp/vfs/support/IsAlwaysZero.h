#pragma once

#include "vfs_namespace.h"

BEG_VFS_NAMESPACE

///
template<class T>
struct IsAlwaysZero {
    enum { value = false };
};

// ::ct_value
template<class T> requires ( T::ct_value() && T::ct_value() == 0 )
struct IsAlwaysZero<T>  {
    enum { value = true };
};

END_VFS_NAMESPACE
