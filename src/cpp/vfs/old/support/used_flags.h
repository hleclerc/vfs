#pragma once

#include "../containers/Vec.h"

BEG_VFS_NAMESPACE

namespace UsedFlags {
    struct Flag { Str name, value; DisplayItem *display( Displayer &ds ) const { return DS_OBJECT( Flag, name, value ); } };
}

extern Vec<UsedFlags::Flag> used_flags;

END_VFS_NAMESPACE
