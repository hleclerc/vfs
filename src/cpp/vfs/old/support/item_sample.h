#pragma once

#include "CtInt.h"

BEG_VFS_NAMESPACE

template<int nb_dims_to_take> constexpr auto &item_sample( const auto &that, CtInt<nb_dims_to_take> ) requires ( requires { that.begin(); } || requires { that[ 0 ]; } ) {
    if constexpr( nb_dims_to_take == 0 ) {
        return that;
    } else if constexpr( nb_dims_to_take == 1 ) {
        if constexpr( requires { that.begin(); } ) {
            ASSERT( that.begin() != that.end() );
            return *that.begin();
        } else {
            ASSERT( that.size() );
            return that[ 0 ];
        }
    } else if constexpr( nb_dims_to_take >= 2 ) {
        return item_sample( item_sample( that, CtInt<1>() ), CtInt<nb_dims_to_take-1>() );
    } else
        static_assert( 0, "don't known how to get an item_sample" );
}

END_VFS_NAMESPACE
