#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

constexpr inline auto nb_bits_mantissa( CtType<PI64> ) { return PrimitiveCtInt<64>(); }
constexpr inline auto nb_bits_mantissa( CtType<PI32> ) { return PrimitiveCtInt<32>(); }
constexpr inline auto nb_bits_mantissa( CtType<PI16> ) { return PrimitiveCtInt<16>(); }
constexpr inline auto nb_bits_mantissa( CtType<PI8 > ) { return PrimitiveCtInt<8 >(); }

constexpr inline auto nb_bits_mantissa( CtType<SI64> ) { return PrimitiveCtInt<63>(); }
constexpr inline auto nb_bits_mantissa( CtType<SI32> ) { return PrimitiveCtInt<31>(); }
constexpr inline auto nb_bits_mantissa( CtType<SI16> ) { return PrimitiveCtInt<15>(); }
constexpr inline auto nb_bits_mantissa( CtType<SI8 > ) { return PrimitiveCtInt<7 >(); }

constexpr inline auto nb_bits_mantissa( CtType<Bool> ) { return PrimitiveCtInt<1 >(); }

// __float80 64
// __float128 =>
constexpr inline auto nb_bits_mantissa( CtType<FP64> ) { return PrimitiveCtInt<52>(); }
constexpr inline auto nb_bits_mantissa( CtType<FP32> ) { return PrimitiveCtInt<25>(); }

END_VFS_NAMESPACE
