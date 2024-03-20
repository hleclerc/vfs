#pragma once

#include "vfs_namespace.h"
#include <algorithm>
#include <string>

BEG_VFS_NAMESPACE

template<std::size_t N>
struct CtStringValue {
    constexpr   CtStringValue( const char (&str)[N] ) { std::copy_n( str, N, value ); }

    std::string to_string    () const { return { value, value + N - 1 }; }

    char        value[N];
};

END_VFS_NAMESPACE
