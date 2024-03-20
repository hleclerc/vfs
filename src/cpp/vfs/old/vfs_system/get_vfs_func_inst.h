#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE
    
void *get_vfs_func_inst(
    const Str &name,
    const Str &return_type,
    Vec<Str> &&arg_types,
    Vec<bool> &&arg_trivs,
    CompilationFlags &&compilation_flags,
    Vec<Vec<Str>> &&final_types,
    Vec<Vec<Str>> &&final_refs,
    Vec<Str> &&cast_types
);

END_VFS_NAMESPACE
