#pragma once

#include "common_macros.h"

BEG_VFS_INTERNAL_NAMESPACE

void destroy( auto &value ) {
    using T = DECAYED_TYPE_OF( value );
    value.~T();
}

END_VFS_INTERNAL_NAMESPACE
