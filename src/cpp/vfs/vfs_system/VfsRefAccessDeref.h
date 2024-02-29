#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

template<class PtrAccess,bool ext>
struct VfsRefAccessDeref {
    static bool   owned() { return ! ext; }
    static Str    code () { return "*( " + PtrAccess::code() +" )"; }
    static auto&& ref  ( auto &&content ) { if constexpr( ext || ! std::is_rvalue_reference_v<decltype(content)> ) return *content; else return std::move( *content ); }
};

END_VFS_NAMESPACE
