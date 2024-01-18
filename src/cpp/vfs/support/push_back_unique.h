#pragma once

#include <algorithm>

namespace Vfs {

template<class Vec,class Val>
auto &push_back_unique( Vec &vec, Val &&val ) {
    auto iter = std::find( vec.begin(), vec.end(), val );
    if ( iter != vec.end() )
        return *iter;
    vec.push_back( std::forward<Val>( val ) );
    return vec.back();
}

}
