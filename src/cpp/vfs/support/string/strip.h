#pragma once

#include <string_view>
#include <string>

namespace Vfs {

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

}
