#pragma once

#include "is_always_zero.h"

BEG_VFS_NAMESPACE

TT constexpr bool is_scalar( CtType<T> t ) {
    return is_always_zero( CT_DECAYED_TYPE_OF( tensor_order( t ) ) );
}

END_VFS_NAMESPACE
