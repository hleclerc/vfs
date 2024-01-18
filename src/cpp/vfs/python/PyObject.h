#pragma once

#include <vfs/support/CompilationFlags.h>
#include <Python.h>

namespace Vfs {

void get_compilation_flags( CompilationFlags &cn, CtType<PyObject> );
Str type_name( CtType<PyObject> );

} // namespace Vfs
