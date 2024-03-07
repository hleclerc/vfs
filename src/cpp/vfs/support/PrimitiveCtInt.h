#pragma once

#include "vfs_namespace.h"
#include <string>

BEG_VFS_NAMESPACE

template<int i>
struct PrimitiveCtInt {
    static void          for_each_template_arg ( auto &&f ) { f( PrimitiveCtInt<i>() ); }
    static auto          template_type_name    () { return "PrimitiveCtInt"; }
    static auto          to_string             () { return std::to_string( value ); }
    static constexpr int value                 = i;
};

template<int a,int b>
constexpr auto max( PrimitiveCtInt<a>, PrimitiveCtInt<b> ) { return PrimitiveCtInt<( a >= b ? a : b )>(); }

END_VFS_NAMESPACE
