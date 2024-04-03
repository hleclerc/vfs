#pragma once

#include "../support/Vec.h"
#include "TdType.h"

BEG_VFS_INTERNAL_NAMESPACE

/// there is a TdTypeTable for each Wrapper type (like Int, Scalar, Type, ...)
struct TdTypeTable {
    PI32          new_type_index  ();
    void          register_type   ( TdType *type );

    PI32          last_type_index = 0;
    PI32          array_size      = 256;
    Vec<TdType *> types;
};

END_VFS_INTERNAL_NAMESPACE
