#pragma once

#include "CtInt.h"

BEG_VFS_NAMESPACE

constexpr auto &item_sample( const auto &that, CtInt<1> nb_dims_to_take ) requires ( requires { that.begin(); } || requires { that[ 0 ]; } ) {
    if constexpr( requires { that.begin(); } ) {
        ASSERT( that.begin() != that.end() );
        return *that.begin();
    } else {
        ASSERT( that.size() );
        return that[ 0 ];
    }
}

END_VFS_NAMESPACE
