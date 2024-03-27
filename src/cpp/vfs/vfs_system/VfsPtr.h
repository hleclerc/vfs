#pragma once

#include "../support/TypeConfig.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

///
struct VfsPtr {
    mutable PI32 instantiated_type_index;
    PI32         global_type_index;
    void*        data;
};

END_VFS_NAMESPACE
