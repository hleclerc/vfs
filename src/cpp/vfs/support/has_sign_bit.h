#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

constexpr inline auto has_sign_bit( CtType<PI64> ) { return CtInt<0>(); }
constexpr inline auto has_sign_bit( CtType<PI32> ) { return CtInt<0>(); }
constexpr inline auto has_sign_bit( CtType<PI16> ) { return CtInt<0>(); }
constexpr inline auto has_sign_bit( CtType<PI8 > ) { return CtInt<0>(); }

constexpr inline auto has_sign_bit( CtType<SI64> ) { return CtInt<1>(); }
constexpr inline auto has_sign_bit( CtType<SI32> ) { return CtInt<1>(); }
constexpr inline auto has_sign_bit( CtType<SI16> ) { return CtInt<1>(); }
constexpr inline auto has_sign_bit( CtType<SI8 > ) { return CtInt<1>(); }

constexpr inline auto has_sign_bit( CtType<Bool> ) { return CtInt<0>(); }

constexpr inline auto has_sign_bit( CtType<FP64> ) { return CtInt<1>(); }
constexpr inline auto has_sign_bit( CtType<FP32> ) { return CtInt<1>(); }

END_VFS_NAMESPACE
