#pragma once

#include "vfs_namespace.h"
#include "CtType.h"

BEG_VFS_NAMESPACE

TT auto *construct( auto &self, CtType<T>, auto &&...ctor_args ) {
    return new ( &self ) T( FORWARD( ctor_args )... );
}

END_VFS_NAMESPACE
