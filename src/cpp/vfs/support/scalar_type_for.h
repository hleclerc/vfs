#pragma once

#include "TypeConfig.h"

namespace Vfs {

template<int scalar_class, int has_sign_bit, int nb_bits_mantissa>
constexpr auto scalar_type_for( CtInt<0> _virtual, CtInt<scalar_class>, CtInt<has_sign_bit>, CtInt<nb_bits_mantissa> ) requires ( scalar_class <= 2 && nb_bits_mantissa <= 64 ) {
    // floating point
    if constexpr ( scalar_class == 2 ) {
        if      ( nb_bits_mantissa <= 25 ) return CtType<FP32>();
        else if ( nb_bits_mantissa <= 52 ) return CtType<FP64>();
        else static_assert( 0, "don't know how to make fp scalar_type_for" );
    } else

    // integer
    if constexpr ( scalar_class == 0 ) {
        if constexpr ( has_sign_bit ) {
            if      ( nb_bits_mantissa <= 8  ) return CtType<PI8 >();
            else if ( nb_bits_mantissa <= 16 ) return CtType<PI16>();
            else if ( nb_bits_mantissa <= 32 ) return CtType<PI32>();
            else if ( nb_bits_mantissa <= 64 ) return CtType<PI64>();
            else static_assert( 0, "don't know how to make PI scalar_type_for" );
        } else {
            // TODO: correct rules for signed integers
            if      ( nb_bits_mantissa <= 8  ) return CtType<SI8 >();
            else if ( nb_bits_mantissa <= 16 ) return CtType<SI16>();
            else if ( nb_bits_mantissa <= 32 ) return CtType<SI32>();
            else if ( nb_bits_mantissa <= 64 ) return CtType<SI64>();
            else static_assert( 0, "don't know how to make PI scalar_type_for" );
        }
    } else

    // default
    static_assert( 0, "don't know how to make scalar_type_for" );
}

}
