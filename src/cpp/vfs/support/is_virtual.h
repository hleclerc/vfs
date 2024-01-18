#pragma once

#include "TypeConfig.h"

namespace Vfs {

constexpr inline auto is_virtual( auto ) { return CtInt<0>(); }

}
