#pragma once

#include "VfsObject.h"
#include "../Int.h"

BEG_VFS_NAMESPACE

///
template<class IntType>
class VfsObject_StdInt : public VfsObject<Int,VfsObject_StdInt<IntType>> {
public:
    /**/    VfsObject_StdInt( IntType value = 0 ) : value( value ) {}

    IntType value;
};

END_VFS_NAMESPACE
