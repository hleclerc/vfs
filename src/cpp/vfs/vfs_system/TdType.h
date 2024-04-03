#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_INTERNAL_NAMESPACE

///
class TdType {
public:
    /**/           TdType      ();
    virtual       ~TdType      ();

    virtual void   register_type() = 0;
    virtual Str    cast_type    () const = 0;

    static TdType* last_unregistered_type;
    TdType*        prev_unregistered_type;
    PI32           type_index; ///<
};


END_VFS_INTERNAL_NAMESPACE
