#pragma once

#include "TypeConfig.h"

namespace Vfs {

TT constexpr auto item_type( CtType<Vec<T>> ) { return CtType<T>(); }

}
