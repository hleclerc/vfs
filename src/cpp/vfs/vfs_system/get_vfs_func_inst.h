#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE
    
void *get_vfs_func_inst( const Str &name, const Str &return_type, Vec<Str> &&arg_types, Vec<bool> &&arg_trivs, Vec<Vec<Str>> &&ct_casts, CompilationFlags &&cn );

END_VFS_NAMESPACE
