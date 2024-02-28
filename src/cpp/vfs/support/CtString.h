#pragma once

#include "CtStringValue.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue str>
struct CtString {
    static auto *display( auto &ds ) { return ds.string( str.value ); }
    static constexpr CtStringValue value = str;
};

END_VFS_NAMESPACE
