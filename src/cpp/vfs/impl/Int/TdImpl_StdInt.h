#pragma once

#include "../../vfs_system/VfsImpl.h"

BEG_VFS_NAMESPACE
class Int;

template<class IntType>
struct VfsImpl_StdInt : VfsImpl<Int,VfsImpl_StdInt<IntType>> {
    /**/         VfsImpl_StdInt( auto &&...ctor_args ) : data( FORWARD( ctor_args )... ) {}

    IntType      data;
};

END_VFS_NAMESPACE
