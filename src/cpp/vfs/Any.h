#pragma once

#include "vfs_system/VfsDtObject.h"

BEG_VFS_NAMESPACE

class Any : public VfsDtObject<> {
public:
    VfsDtObject_STD_METHODS( Any, "vfs" );

    /**/ Any( auto &&value ) : Any( FromValue(), FORWARD( value ) ) {}
    /**/ Any() : Any( Void() ) {}
};

END_VFS_NAMESPACE
