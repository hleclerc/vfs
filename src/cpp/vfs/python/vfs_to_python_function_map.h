#pragma once

#include <vfs/support/TypeConfig.h>
#include <map>

namespace Vfs {

/// cpp type => include to find to find the right vfs_to_python function
extern std::map<Str,Str> vfs_to_python_function_map;

} // namespace Vfs
