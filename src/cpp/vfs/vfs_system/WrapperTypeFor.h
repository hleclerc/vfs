#pragma once

#include "../support/vfs_namespace.h"
#include <type_traits>

BEG_VFS_NAMESPACE
struct VfsWrapper;

/// ex: SI32 => Int, FP64 => Scalar, ...
template<class T> struct VfsWrapperTypeFor;

/// ex: SI32 => Int, FP64 => Scalar, ...
template<class T> requires std::is_base_of_v<VfsWrapper,T> struct VfsWrapperTypeFor<T> { using value = T; };

END_VFS_NAMESPACE
