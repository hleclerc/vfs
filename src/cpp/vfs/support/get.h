#pragma once

#include "common_macros.h"

BEG_VFS_NAMESPACE

TT requires requires ( const T &v ) { v.get_lvalue(); }
const auto &get( const T &v ) { return v.get_lvalue(); }

TT requires requires ( T &&v ) { v.get_rvalue(); }
auto &&get( T &&v ) { return v.get_rvalue(); }

END_VFS_NAMESPACE
