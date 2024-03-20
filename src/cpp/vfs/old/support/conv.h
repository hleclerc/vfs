#pragma once

#include "CtType.h"

BEG_VFS_NAMESPACE

template<class T> T conv( CtType<T>, auto &&v ) { return FORWARD( v ); }

END_VFS_NAMESPACE
