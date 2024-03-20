#pragma once

#include "../support/vfs_namespace.h"

BEG_VFS_NAMESPACE

/// Vfs argument trait. Defines how VFS_CALL must handle a VfsArg type
template<class T,bool free_parameter=true>
struct VfsArgTrait;

END_VFS_NAMESPACE
