#pragma once

#include "../vfs_namespace.h"
#include <string_view>
#include <string>

BEG_VFS_NAMESPACE

inline std::string_view strip( std::string_view res ) {
    while ( res.size() && res.back() == ' ' )
        res.remove_suffix( 1 );
    while ( res.size() && res.front() == ' ' )
        res.remove_prefix( 1 );
    return res;
}

inline void strip_inplace( std::string &res ) {
    while ( res.size() && res.back() == ' ' )
        res.pop_back();
    while ( res.size() && res.front() == ' ' )
        res = res.substr( 1 ); // TODO: optimize
}

END_VFS_NAMESPACE
