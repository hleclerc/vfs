#pragma once

#include "../Type.h"

BEG_VFS_NAMESPACE

void impl_type( Type &type_wrapper, auto &&value ) {
    new ( &type_wrapper ) Type( CT_DECAYED_TYPE_OF( value ) );
}

END_VFS_NAMESPACE
