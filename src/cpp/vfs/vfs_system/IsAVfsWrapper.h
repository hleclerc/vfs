#pragma once

#include "../support/vfs_namespace.h"

BEG_VFS_NAMESPACE

template<class T>
concept IsAVfsObject = requires ( const T &arg ) { arg.__vfs_wrapper_attributes; };

END_VFS_NAMESPACE
