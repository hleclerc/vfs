#pragma once

#include "../TypeConfig.h"

namespace Vfs {

Vec<Str> split( StrView str, const Str &sep = " " );

TT Vec<Str> split( StrView str, const Str &sep, const T &func ) {
    Vec<Str> res;

    while ( true ) {
        auto p = str.find( sep );
        if ( p == str.npos ) {
            res.push_back( func( str ) );
            break;
        }

        res.push_back( func( str.substr( 0, p ) ) );
        str.remove_prefix( p + sep.size() );
    }

    return res;
}

}
