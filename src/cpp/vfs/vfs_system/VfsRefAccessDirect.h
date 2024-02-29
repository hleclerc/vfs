#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

struct VfsRefAccessDirect {
    static auto   type_name() { return "VfsRefAccessDirect"; }
    static bool   owned    () { return true; }
    static Str    code     () { return "{CONTENT}"; }
    static auto&& ref      ( auto &&value ) { return FORWARD( value ); }
};

END_VFS_NAMESPACE
