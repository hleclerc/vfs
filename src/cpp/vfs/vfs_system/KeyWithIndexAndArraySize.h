#pragma once

#include "../support/common_macros.h"

BEG_VFS_INTERNAL_NAMESPACE

/// base class for keys that support indexing in arrays
struct KeyWithIndexAndArraySize {
};

TT concept IsAKeyWithIndexAndArraySize = std::is_base_of_v<KeyWithIndexAndArraySize,std::decay_t<T>>;

END_VFS_INTERNAL_NAMESPACE
