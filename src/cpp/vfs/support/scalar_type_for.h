#pragma once

#include "STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "TypeConfig.h"

BEG_VFS_NAMESPACE

template<int scalar_class, int has_sign_bit, int nb_bits_mantissa>
constexpr auto scalar_type_for( PrimitiveCtInt<0> _virtual, PrimitiveCtInt<scalar_class>, PrimitiveCtInt<has_sign_bit>, PrimitiveCtInt<nb_bits_mantissa> ) requires ( scalar_class <= 2 && nb_bits_mantissa <= 64 ) {
    // floating point
    if constexpr ( scalar_class == 2 ) {
        if      constexpr ( nb_bits_mantissa <= 25 ) return CtType<FP32>();
        else if constexpr ( nb_bits_mantissa <= 52 ) return CtType<FP64>();
        else STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( CtType<FP32>(), "..." );
    } else

    // integer
    if constexpr ( scalar_class == 0 ) {
        if constexpr ( has_sign_bit ) {
            if      constexpr ( nb_bits_mantissa <= 8  ) return CtType<PI8 >();
            else if constexpr ( nb_bits_mantissa <= 16 ) return CtType<PI16>();
            else if constexpr ( nb_bits_mantissa <= 32 ) return CtType<PI32>();
            else if constexpr ( nb_bits_mantissa <= 64 ) return CtType<PI64>();
            else STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( CtType<PI32>(), "..." );
        } else {
            // TODO: correct rules for signed integers
            if      constexpr ( nb_bits_mantissa <= 8  ) return CtType<SI8 >();
            else if constexpr ( nb_bits_mantissa <= 16 ) return CtType<SI16>();
            else if constexpr ( nb_bits_mantissa <= 32 ) return CtType<SI32>();
            else if constexpr ( nb_bits_mantissa <= 64 ) return CtType<SI64>();
            else STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( CtType<SI32>(), "..." );
        }
    } else

    // default
    STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( CtType<SI32>(), "don't know how to make scalar_type_for" );
}

END_VFS_NAMESPACE
