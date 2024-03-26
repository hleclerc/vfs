#pragma once

#include "../vfs_system/VfsObject.h"
class Int;

BEG_VFS_NAMESPACE

///
template<class IntType>
class VfsInt : public VfsObject<Int,VfsInt<IntType>> {
public:
    /**/    VfsInt( IntType value = 0 ) : value( value ) {}

    IntType value;
};

END_VFS_NAMESPACE
