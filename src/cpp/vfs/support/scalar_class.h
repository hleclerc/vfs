// 0 -> int like; 1 -> rationnal; 2 -> floating point
#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

constexpr inline auto scalar_class( CtType<PI64> ) { return CtInt<0>(); }
constexpr inline auto scalar_class( CtType<PI32> ) { return CtInt<0>(); }
constexpr inline auto scalar_class( CtType<PI16> ) { return CtInt<0>(); }
constexpr inline auto scalar_class( CtType<PI8 > ) { return CtInt<0>(); }

constexpr inline auto scalar_class( CtType<SI64> ) { return CtInt<0>(); }
constexpr inline auto scalar_class( CtType<SI32> ) { return CtInt<0>(); }
constexpr inline auto scalar_class( CtType<SI16> ) { return CtInt<0>(); }
constexpr inline auto scalar_class( CtType<SI8 > ) { return CtInt<0>(); }

constexpr inline auto scalar_class( CtType<Bool> ) { return CtInt<0>(); }

constexpr inline auto scalar_class( CtType<FP64> ) { return CtInt<2>(); }
constexpr inline auto scalar_class( CtType<FP32> ) { return CtInt<2>(); }

END_VFS_NAMESPACE
