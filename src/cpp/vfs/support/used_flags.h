#pragma once

#include "Vec.h"

BEG_VFS_NAMESPACE

namespace UsedFlags {
    struct Flag {
        DisplayItem *display( Displayer &ds ) const {
            return DS_OBJECT( Flag, name, value );
        }

        Str name, value;
    };
}

extern Vec<UsedFlags::Flag> used_flags;

END_VFS_NAMESPACE
