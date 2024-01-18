#pragma once

#include "../Opt.h"
#include <sstream>
#include <fstream>
#include <string>

namespace Vfs {

inline Opt<std::string> read_file( const std::string_view &filename ) {
    std::ifstream fi( std::string{ filename } );
    if ( ! fi )
        return {};
    std::stringstream is;
    is << fi.rdbuf();
    return is.str();
}

} // namespace Vfs
