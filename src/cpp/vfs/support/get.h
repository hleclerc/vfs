#pragma once

#include "common_macros.h"

BEG_VFS_NAMESPACE

TT requires requires ( const T &v ) { v.lvalue(); }
const auto &get( const T &v ) { return v.lvalue(); }

TT requires requires ( T &&v ) { v.rvalue(); }
auto &&get( T &&v ) { return v.rvalue(); }

END_VFS_NAMESPACE
