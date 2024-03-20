#include "PyObject.h"

BEG_VFS_NAMESPACE

void get_compilation_flags( CompilationFlags &cn, CtType<PyObject> ) {
    cn.add_inc_file( "vfs/python/PyObject.h" );
}

Str type_name( CtType<PyObject> ) {
    return "PyObject";
}

END_VFS_NAMESPACE
