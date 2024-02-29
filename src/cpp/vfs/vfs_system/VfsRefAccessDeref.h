#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

template<class PtrAccess,bool ext>
struct VfsRefAccessDeref {
    static bool  owned() { return ! ext; }
    static Str   code () { return "*( " + PtrAccess::code() +" )"; }
    static auto& ref  ( auto &content ) { return *content; }
};

END_VFS_NAMESPACE
