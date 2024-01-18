#pragma once

#include <algorithm>
#include <string>

namespace Vfs {

template<std::size_t N>
struct CtStringValue {
    constexpr   CtStringValue( const char (&str)[N] ) { std::copy_n( str, N, value ); }

    std::string to_string    () const { return { value, value + N - 1 }; }

    char        value[N];
};

}
