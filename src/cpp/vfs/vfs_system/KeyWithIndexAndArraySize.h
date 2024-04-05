#pragma once

#include "../support/common_macros.h"

BEG_VFS_NAMESPACE

/// base class for keys that support indexing in arrays
struct VfsKeyWithIndexAndArraySize {
};

TT concept IsAVfsKeyWithIndexAndArraySize = std::is_base_of_v<VfsKeyWithIndexAndArraySize,std::decay_t<T>>;

END_VFS_NAMESPACE
