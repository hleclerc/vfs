#pragma once

#include "Type.h"

BEG_VFS_NAMESPACE

class Scalar { VFS_TD_ATTRIBUTES( Scalar, "vfs" );
public:
};

// types for ctors --------------------------------------------------------------------------
auto vfs_td_impl_type( CtType<Scalar>, const ScalarLike auto &value ) {
    return CT_DECAYED_TYPE_OF( value );
}

inline auto vfs_td_impl_type( CtType<Scalar> ) {
    return CtType<CtInt<0>>();
}


// -------------------------------------------------------------------------------------------
// type info
constexpr inline auto nb_bits_mantissa( CtType<Scalar> ) { return CtInt<1000000>(); }
constexpr inline auto scalar_class    ( CtType<Scalar> ) { return CtInt<2>(); }
constexpr inline auto has_sign_bit    ( CtType<Scalar> ) { return CtInt<1>(); }

// scalar_type_for (when creating "virtual" objects from "real" values)
auto scalar_type_for( CtInt<1>, auto, auto, auto ) { return CtType<Scalar>(); }

END_VFS_NAMESPACE
