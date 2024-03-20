#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

constexpr inline auto is_virtual( auto ) { return CtInt<0>(); }

END_VFS_NAMESPACE
