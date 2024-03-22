#pragma once

#include "default_operators.h"

BEG_VFS_NAMESPACE

constexpr auto min( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( min ) return a <= b ? FORWARD( a ) : FORWARD( b ); }

END_VFS_NAMESPACE
