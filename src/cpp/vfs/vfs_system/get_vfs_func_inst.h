#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

void *get_vfs_func_inst( const Str &name, const Str &return_type, Seq<Str> &&arg_types, Seq<bool> &&arg_trivs, Seq<Seq<Str>> &&ct_casts, CompilationFlags &&cn );

END_VFS_NAMESPACE
