#pragma once

#include "CtInt.h"

BEG_VFS_NAMESPACE
    
constexpr auto item_type( const auto &that, CtInt<1> nb_dims_to_take ) requires ( requires { that.begin(); } || requires { that[ 0 ]; } ) {
    if constexpr( requires { that.begin(); } )
        return CT_DECAYED_TYPE_OF( *that.begin() );
    else
        return CT_DECAYED_TYPE_OF( that[ 0 ] );
}

END_VFS_NAMESPACE
