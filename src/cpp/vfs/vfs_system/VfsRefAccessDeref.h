#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

template<class PtrAccess,bool ext>
struct VfsRefAccessDeref {
    static void   for_each_template_arg( auto &&f ) { f( CtType<PtrAccess>() ); f( CtInt<ext>() ); }
    static auto   template_type_name   () { return "VfsRefAccessDeref"; }
    static bool   owned                () { return ! ext; }
    static Str    code                 () { return Str( ext ? "" : "{BEG_ARG_FORWARD}" ) + "*( " + PtrAccess::code() +" )" + Str( ext ? "" : "{END_ARG_FORWARD}" ); }
    static auto&& ref                  ( auto &&content ) { if constexpr( ext || ! std::is_rvalue_reference_v<decltype(content)> ) return *content; else return std::move( *content ); }
};

END_VFS_NAMESPACE
