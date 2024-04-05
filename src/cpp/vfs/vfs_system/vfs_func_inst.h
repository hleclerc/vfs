#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

///
void *vfs_func_inst( const Str &name, const CompilationFlags &cf, const Str &return_type, PI nb_args, const Str *arg_types, const Str *cast_types );

END_VFS_NAMESPACE
