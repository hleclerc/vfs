#pragma once

#include "vfs_namespace.h"

BEG_VFS_NAMESPACE

/**
 * Used to make singleton that are initialized once at loading time
*/
template<class T,class...>
struct StaticStorage {
    static T value;
};

template<class T,class...A>
T StaticStorage<T,A...>::value;

END_VFS_NAMESPACE
