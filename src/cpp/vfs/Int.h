#pragma once

#include "Type.h"

BEG_VFS_NAMESPACE

class Int { VFS_DT_OBJECT_ATTRIBUTES( Int, "vfs" );
public:
    /**/ Int( ScalarLike auto &&value ) : Int( FromValue(), FORWARD( value ) ) {}
    /**/ Int() : Int( CtInt<0>() ) {}
};

// -------------------------------------------------------------------------------------------
// type info
constexpr inline auto nb_bits_mantissa( CtType<Int> ) { return PrimitiveCtInt<1000000>(); }
constexpr inline auto scalar_class    ( CtType<Int> ) { return PrimitiveCtInt<0>(); }
constexpr inline auto has_sign_bit    ( CtType<Int> ) { return PrimitiveCtInt<1>(); }

// scalar_type_for (when creating "virtual" objects from "real" values)
auto scalar_type_for( PrimitiveCtInt<1> /*virtual*/, CtInt<0> /*scalar class*/, auto, auto ) { return CtType<Int>(); }

END_VFS_NAMESPACE
