#include "PyObject.h"

namespace Vfs {

void get_compilation_flags( CompilationFlags &cn, CtType<PyObject> ) {
    cn.add_inc_file( "vfs/python/PyObject.h" );
}

Str type_name( CtType<PyObject> ) {
    return "PyObject";
}

} // namespace Vfs
