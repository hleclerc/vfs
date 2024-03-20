#pragma once

#include "CtStringValue.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue str>
struct CtString {
    static void                    for_each_template_arg( auto &&f ) { f( CtString<value>() ); }
    static auto                    template_type_name   () { return "CtString"; }
    static auto                    to_string            () { return "\"" + str.to_string() + "\""; }
    static auto*                   display              ( auto &ds ) { return ds.string( str.value ); }
    static constexpr CtStringValue value                = str;
};

END_VFS_NAMESPACE
