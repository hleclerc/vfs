#pragma once

#include "../../vfs_system/VfsImpl.h"
#include "../../Int.h"

BEG_VFS_NAMESPACE

template<class IntType>
struct VfsImpl_StdInt : VfsImpl<Int,VfsImpl_StdInt<IntType>> {
    /**/         VfsImpl_StdInt( auto &&...ctor_args ) : data( FORWARD( ctor_args )... ) {}

    IntType      data;
};

END_VFS_NAMESPACE
