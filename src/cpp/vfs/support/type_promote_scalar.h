#pragma once

#include "nb_bits_mantissa.h"
#include "scalar_type_for.h"
#include "ScalarClass.h"
#include "has_sign_bit.h"
#include "CtString.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue op,class T_A,class T_B>
constexpr auto type_promote_scalar( CtString<op>, CtType<T_A> a, CtType<T_B> b ) {
    return scalar_type_for(
        max( PrimitiveCtInt<VfsArg<T_A>>(), PrimitiveCtInt<VfsArg<T_B>>() ),
        max( scalar_class( a ), scalar_class( b ) ),
        max( has_sign_bit( a ), has_sign_bit( b ) ),
        max( nb_bits_mantissa( a ), nb_bits_mantissa( b ) )
    );
}

END_VFS_NAMESPACE
