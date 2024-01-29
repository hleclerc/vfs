#pragma once

#include "CtStringValue.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue str>
struct CtString {
    static constexpr CtStringValue value = str;
};

END_VFS_NAMESPACE
