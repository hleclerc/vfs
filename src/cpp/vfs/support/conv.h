#pragma once

#include "CtType.h"

namespace Vfs {

template<class T> T conv( CtType<T>, auto &&v ) { return FORWARD( v ); }

} // namespace Vfs
