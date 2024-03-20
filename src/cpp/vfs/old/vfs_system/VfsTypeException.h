#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

struct VfsTypeException {
    virtual ~VfsTypeException() {}
    virtual void change_type_of( void *object ) const = 0;
};

END_VFS_NAMESPACE
