#pragma once

#include "Type.h"

BEG_VFS_NAMESPACE

class Any {
public:
    VfsDtObject_STD_METHODS( Any, "vfs", sizeof( void * ), alignof( void * ) );

    /**/ Any( auto &&value ) : Any( FromValue(), FORWARD( value ) ) {}
    /**/ Any() : Any( Void() ) {}
};

END_VFS_NAMESPACE
