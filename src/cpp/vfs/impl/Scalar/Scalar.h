#pragma once

#include "../../support/WithDefaultOperators.h"
#include "VfsTdImpl_StdScalar.h"
#include "../../Type.h"
// #include "Type.h"

BEG_VFS_NAMESPACE

/// Wrap an integer
class Scalar : public VfsTdWrapper<Scalar,sizeof(PI64),alignof(PI64)>, WithDefaultOperators {
public:
    STD_METHODS_FOR_VFS_TD_WRAPPER( Scalar, "VFS_NAMESPACE", "vfs" );
};

// types for ctors --------------------------------------------------------------------------
TT requires std::is_scalar_v<T> struct VfsTdImplFor<Scalar,T> { using value = VfsTdImpl_StdScalar<Scalar,typename StorageTypeFor<T>::value>; };
template<> struct VfsTdImplFor<Scalar> { using value = VfsTdImpl_StdScalar<Scalar,CtInt<0>>; };

//
TT requires ( std::is_scalar_v<T> && ! std::is_integral_v<T> ) struct VfsWrapperTypeFor<T> { using value = Scalar; };

// // -------------------------------------------------------------------------------------------
// // type info
// constexpr inline auto nb_bits_mantissa( CtType<Scalar> ) { return PrimitiveCtInt<1000000>(); }
// constexpr inline auto scalar_class    ( CtType<Scalar> ) { return PrimitiveCtInt<2>(); }
// constexpr inline auto has_sign_bit    ( CtType<Scalar> ) { return PrimitiveCtInt<1>(); }

// // scalar_type_for (when creating "virtual" objects from "real" values)
// auto scalar_type_for( PrimitiveCtInt<1>, auto, auto, auto ) { return CtType<Scalar>(); }

END_VFS_NAMESPACE

