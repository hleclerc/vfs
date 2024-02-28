#pragma once

#include "CtString.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

///
template<CtStringValue... values>
struct CtStringList {
    static void  for_each_string( auto &&f ) { ( f( values ), ... ); }
    static auto *display        ( auto &ds ) { return ds.array( { ds.string( values.value )... } ); }
};

END_VFS_NAMESPACE
