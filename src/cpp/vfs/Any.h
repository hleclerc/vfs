#pragma once

#include "vfs_system/VfsTdObject.h"

BEG_VFS_NAMESPACE

class Any { VFS_TD_OBJECT_ATTRIBUTES( Any, "vfs" );
public:
    /**/ Any( auto &&value ) : Any( FromValue(), FORWARD( value ) ) {}
    /**/ Any() : Any( Void() ) {}
};

END_VFS_NAMESPACE
