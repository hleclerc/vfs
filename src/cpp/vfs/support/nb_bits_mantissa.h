#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

constexpr inline auto nb_bits_mantissa( CtType<PI64> ) { return CtInt<64>(); }
constexpr inline auto nb_bits_mantissa( CtType<PI32> ) { return CtInt<32>(); }
constexpr inline auto nb_bits_mantissa( CtType<PI16> ) { return CtInt<16>(); }
constexpr inline auto nb_bits_mantissa( CtType<PI8 > ) { return CtInt<8 >(); }

constexpr inline auto nb_bits_mantissa( CtType<SI64> ) { return CtInt<63>(); }
constexpr inline auto nb_bits_mantissa( CtType<SI32> ) { return CtInt<31>(); }
constexpr inline auto nb_bits_mantissa( CtType<SI16> ) { return CtInt<15>(); }
constexpr inline auto nb_bits_mantissa( CtType<SI8 > ) { return CtInt<7 >(); }

constexpr inline auto nb_bits_mantissa( CtType<Bool> ) { return CtInt<1 >(); }

// __float80 => 64
// __float128 =>
constexpr inline auto nb_bits_mantissa( CtType<FP64> ) { return CtInt<52>(); }
constexpr inline auto nb_bits_mantissa( CtType<FP32> ) { return CtInt<25>(); }

END_VFS_NAMESPACE
