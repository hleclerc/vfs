#pragma once

#include "../support/StorageTypeFor.h"

BEG_VFS_NAMESPACE

/// Td impl
template<class Wrapper,class... CtorArgs>
struct VfsTdImplFor;

/// Td impl is StorageTypeFor is !=
template<class Wrapper,class T> requires ( std::is_same_v<typename StorageTypeFor<T>::value,T> == false && requires { (typename VfsTdImplFor<Wrapper,typename StorageTypeFor<T>::value>::value *)nullptr; } )
struct VfsTdImplFor<Wrapper,T> {
    using value = VfsTdImplFor<Wrapper,typename StorageTypeFor<T>::value>::value;
};

END_VFS_NAMESPACE
