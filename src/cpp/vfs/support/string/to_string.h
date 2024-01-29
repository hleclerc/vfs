#pragma once

#include "../display/Displayer.h"
#include <sstream>

BEG_VFS_NAMESPACE

template<class T>
std::string to_string( T &&val, const DisplayParameters &dp = {} ) {
    Displayer ds;
    std::ostringstream ss;
    display( ds, std::forward<T>( val ) )->write_to( ss, dp );

    return ss.str();
}

END_VFS_NAMESPACE
