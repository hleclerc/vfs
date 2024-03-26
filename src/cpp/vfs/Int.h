#pragma once

#include "support/WithDefaultOperators.h"
#include "vfs_system/VfsWrapper.h"
#include "impl/VfsInt.h"
// #include "Type.h"

BEG_VFS_NAMESPACE

class Int : public WithDefaultOperators {
public:
    STD_METHODS_FOR_VFS_WRAPPER( Int, "VFS_NAMESPACE", "vfs", sizeof( void * ), alignof( void * ) );
};

// types for ctors --------------------------------------------------------------------------
TT requires std::is_integral_v<T> struct VfsStorageTypeFor<Int,T> { using value = VfsInt<T>; };
template<> struct VfsStorageTypeFor<Int> { using value = VfsInt<CtInt<0>>; };


// // -------------------------------------------------------------------------------------------
// // type info
// constexpr inline auto nb_bits_mantissa( CtType<Scalar> ) { return PrimitiveCtInt<1000000>(); }
// constexpr inline auto scalar_class    ( CtType<Scalar> ) { return PrimitiveCtInt<2>(); }
// constexpr inline auto has_sign_bit    ( CtType<Scalar> ) { return PrimitiveCtInt<1>(); }

// // scalar_type_for (when creating "virtual" objects from "real" values)
// auto scalar_type_for( PrimitiveCtInt<1>, auto, auto, auto ) { return CtType<Scalar>(); }

END_VFS_NAMESPACE
