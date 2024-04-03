#pragma once

#include "vfs_namespace.h"
#include <string>

BEG_VFS_NAMESPACE

template<class T>
struct StorageTypeFor {
    using value = T;
};

// reference
template<class T> struct StorageTypeFor<const T *> { using value = StorageTypeFor<T *>::value; };
template<class T> struct StorageTypeFor<const T> { using value = StorageTypeFor<T>::value; };
template<class T> struct StorageTypeFor<T &&> { using value = StorageTypeFor<T>::value; };
template<class T> struct StorageTypeFor<T &> { using value = StorageTypeFor<T>::value; };

// strings
template<std::size_t n> struct StorageTypeFor<char[n]> { using value = std::string; };
template<> struct StorageTypeFor<char *> { using value = std::string; };

END_VFS_NAMESPACE
