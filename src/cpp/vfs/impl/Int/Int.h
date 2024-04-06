#pragma once

#include "../../support/WithDefaultOperators.h"
#include "../../vfs_system/WrapperTypeFor.h"
#include "../../vfs_system/VfsTdWrapper.h"
#include "VfsTdImpl_StdInt.h"
// #include "Type.h"

BEG_VFS_NAMESPACE

/// Wrap an integer
class Int : public VfsTdWrapper<Int,sizeof(PI64),alignof(PI64)>, WithDefaultOperators {
public:
    STD_METHODS_FOR_VFS_TD_WRAPPER( Int, "VFS_NAMESPACE", "vfs" );
};

// types for ctors --------------------------------------------------------------------------
TT requires std::is_integral_v<T> struct VfsTdImplFor<Int,T> { using value = VfsTdImpl_StdInt<Int,typename StorageTypeFor<T>::value>; };
template<> struct VfsTdImplFor<Int> { using value = VfsTdImpl_StdInt<Int,CtInt<0>>; };

//
TT requires std::is_integral_v<T> struct VfsWrapperTypeFor<T> { using value = Int; };

// // -------------------------------------------------------------------------------------------
// // type info
// constexpr inline auto nb_bits_mantissa( CtType<Scalar> ) { return PrimitiveCtInt<1000000>(); }
// constexpr inline auto scalar_class    ( CtType<Scalar> ) { return PrimitiveCtInt<2>(); }
// constexpr inline auto has_sign_bit    ( CtType<Scalar> ) { return PrimitiveCtInt<1>(); }

// // scalar_type_for (when creating "virtual" objects from "real" values)
// auto scalar_type_for( PrimitiveCtInt<1>, auto, auto, auto ) { return CtType<Scalar>(); }

END_VFS_NAMESPACE

