#pragma once

#include "../support/StorageTypeFor.h"

BEG_VFS_INTERNAL_NAMESPACE

/// Td impl
template<class Wrapper,class... CtorArgs>
struct TdImplFor;

/// Td impl is StorageTypeFor is !=
template<class Wrapper,class T> requires ( std::is_same_v<typename StorageTypeFor<T>::value,T> == false && requires { (typename TdImplFor<Wrapper,typename StorageTypeFor<T>::value>::value *)nullptr; } )
struct TdImplFor<Wrapper,T> {
    using value = TdImplFor<Wrapper,typename StorageTypeFor<T>::value>::value;
};

END_VFS_INTERNAL_NAMESPACE
