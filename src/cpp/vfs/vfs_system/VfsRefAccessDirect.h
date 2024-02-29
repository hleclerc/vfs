#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

struct VfsRefAccessOnDtContent {
    static bool  owned() { return true; }
    static Str   code () { return "{CONTENT}.data.content"; }
    static auto& ref  ( auto &cast ) { return cast.data.content; }
};

END_VFS_NAMESPACE
