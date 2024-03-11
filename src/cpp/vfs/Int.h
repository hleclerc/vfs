#pragma once

#include "Type.h"

BEG_VFS_NAMESPACE

class Int {
public:
    VfsDtObject_STD_METHODS( Int, "vfs", sizeof( void * ), alignof( void * ) );
};

// types for ctors --------------------------------------------------------------------------
inline auto vfs_dt_impl_type( CtType<Int>, const ScalarLike auto &value ) { return CT_DECAYED_TYPE_OF( value ); }
inline auto vfs_dt_impl_type( CtType<Int> ) { return CtType<CtInt<0>>(); }

// -------------------------------------------------------------------------------------------
// type info
constexpr inline auto nb_bits_mantissa( CtType<Int> ) { return PrimitiveCtInt<1000000>(); }
constexpr inline auto scalar_class    ( CtType<Int> ) { return PrimitiveCtInt<0>(); }
constexpr inline auto has_sign_bit    ( CtType<Int> ) { return PrimitiveCtInt<1>(); }

// scalar_type_for (when creating "virtual" objects from "real" values)
auto scalar_type_for( PrimitiveCtInt<1> /*virtual*/, CtInt<0> /*scalar class*/, auto, auto ) { return CtType<Int>(); }

// ensure i is a compile time known value
inline Int make_ct_value( const Int &i ) { return VFS_CALL( make_ct_value, CtStringList<>, Int, i ); }

END_VFS_NAMESPACE
