#pragma once

#include "CtType.h"

BEG_VFS_NAMESPACE

///
template<class T>
constexpr bool is_always_zero( CtType<T> ) {
    if constexpr( requires { T::ct_value(); } ) {
        if constexpr( T::ct_value() == 0 )
            return true;
        else
            return false;
    } else
        return false;
}

END_VFS_NAMESPACE
