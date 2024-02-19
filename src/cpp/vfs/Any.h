#pragma once

#include "vfs_system/VfsDtObject.h"

BEG_VFS_NAMESPACE

class Any {
public:
    /**/ VFS_DT_OBJECT_ATTRIBUTES( Any, "vfs" );

    /**/ Any( auto &&value ) : Any( FromValue(), FORWARD( value ) ) {}
    /**/ Any() : Any( Void() ) {}
};

END_VFS_NAMESPACE
