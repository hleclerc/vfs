#pragma once

#include "../support/vfs_namespace.h"
#include <type_traits>

BEG_VFS_NAMESPACE

//
template<class Wrapper,class... CtorArgs>
struct VfsObjectTypeFor;

//
template<class Wrapper,class Arg> requires ( std::is_same_v<std::decay_t<Arg>,Arg> == false ) && requires ( VfsObjectTypeFor<Wrapper,std::decay_t<Arg>>::value *v ) { v; }
struct VfsObjectTypeFor<Wrapper,Arg> {
    using value = VfsObjectTypeFor<Wrapper,std::decay_t<Arg>>::value;
};


END_VFS_NAMESPACE
