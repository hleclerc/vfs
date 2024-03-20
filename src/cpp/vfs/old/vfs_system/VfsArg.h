#pragma once

#include "VfsArgTrait.h"

BEG_VFS_NAMESPACE

/// `T` is a `VfsArg` if there's a valid `VfsArgTrait` defined for type `T`
template<class T>
concept VfsArg = requires ( const T &obj ) {
    VfsArgTrait<T>::key( obj );
};

END_VFS_NAMESPACE
