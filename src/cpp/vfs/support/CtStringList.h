#pragma once

#include "CtString.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

///
template<CtStringValue... values>
struct CtStringList {
    static void for_each_string( auto &&f ) { ( f( values ), ... ); }
};

END_VFS_NAMESPACE
