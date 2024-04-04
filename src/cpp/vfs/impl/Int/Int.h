#pragma once

#include "../../support/WithDefaultOperators.h"
#include "../../vfs_system/TdWrapper.h"
// #include "impl/VfsInt.h"
// #include "Type.h"

BEG_VFS_NAMESPACE

/// Wrap an integer
class Int : public VFS_INTERNAL_NAMESPACE::TdWrapper<Int,sizeof(PI64),alignof(PI64)>, WithDefaultOperators {
public:
    STD_METHODS_FOR_VFS_TD_WRAPPER( Int, "VFS_NAMESPACE", "vfs" );
};

// types for ctors --------------------------------------------------------------------------
// TT requires std::is_integral_v<T> struct VfsObjectTypeFor<Int,T> { using value = VfsInt<T>; };
// template<> struct VfsObjectTypeFor<Int> { using value = VfsInt<CtInt<0>>; };


// // -------------------------------------------------------------------------------------------
// // type info
// constexpr inline auto nb_bits_mantissa( CtType<Scalar> ) { return PrimitiveCtInt<1000000>(); }
// constexpr inline auto scalar_class    ( CtType<Scalar> ) { return PrimitiveCtInt<2>(); }
// constexpr inline auto has_sign_bit    ( CtType<Scalar> ) { return PrimitiveCtInt<1>(); }

// // scalar_type_for (when creating "virtual" objects from "real" values)
// auto scalar_type_for( PrimitiveCtInt<1>, auto, auto, auto ) { return CtType<Scalar>(); }

END_VFS_NAMESPACE

#include "TdImpl_StdInt.h"
