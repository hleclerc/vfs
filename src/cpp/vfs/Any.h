#pragma once

#include "vfs_system/VfsTd.h"

BEG_VFS_NAMESPACE

class Any { VFS_TD_ATTRIBUTES( Any, "vfs" );
public:
    /**/ Any( auto &&value ) : Any( FromValue(), FORWARD( value ) ) {}
    Any() : Any( Void() ) {}
};

END_VFS_NAMESPACE
