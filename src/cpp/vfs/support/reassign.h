#pragma once

#include "ScalarClass.h"
#include "CtInt.h"

BEG_VFS_NAMESPACE

/// like operator= with a check that throw an exception to change the underlying type
auto reassign( auto &dst, auto &&src ) {
    // same types
    if constexpr ( std::is_same_v<DECAYED_TYPE_OF( dst ),DECAYED_TYPE_OF( src )> ) {
        dst = src;
        return CtInt<1>{};
    } else

    //
    if constexpr ( requires { scalar_class( CT_DECAYED_TYPE_OF( dst ) ); scalar_class( CT_DECAYED_TYPE_OF( src ) ); } && scalar_class( CT_DECAYED_TYPE_OF( dst ) ) >= scalar_class( CT_DECAYED_TYPE_OF( src ) ) ) {
        dst = src;
        return CtInt<1>{};
    } else

    return CtInt<0>{};
}

END_VFS_NAMESPACE
