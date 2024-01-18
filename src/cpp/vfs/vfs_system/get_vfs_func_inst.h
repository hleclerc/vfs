#pragma once

#include "../support/CompilationFlags.h"

namespace Vfs {

void *get_vfs_func_inst( const Str &name, const Str &return_type, Vec<Str> &&arg_types, Vec<bool> &&arg_trivs, Vec<Vec<Str>> &&ct_casts, CompilationFlags &&cn );

}
