#pragma once

#include <vfs/support/CompilationFlags.h>
#include <Python.h>

BEG_VFS_NAMESPACE

void get_compilation_flags( CompilationFlags &cn, CtType<PyObject> );
Str type_name( CtType<PyObject> );

END_VFS_NAMESPACE
