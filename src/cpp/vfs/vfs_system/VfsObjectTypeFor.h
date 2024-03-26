#pragma once

#include "../support/vfs_namespace.h"
#include <type_traits>

BEG_VFS_NAMESPACE

//
template<class Wrapper,class... CtorArgs>
struct VfsStorageTypeFor;

//
template<class Wrapper,class Arg> requires ( std::is_same_v<std::decay_t<Arg>,Arg> == false ) && requires ( VfsStorageTypeFor<Wrapper,std::decay_t<Arg>>::value *v ) { v; }
struct VfsStorageTypeFor<Wrapper,Arg> {
    using value = VfsStorageTypeFor<Wrapper,std::decay_t<Arg>>::value;
};


END_VFS_NAMESPACE
