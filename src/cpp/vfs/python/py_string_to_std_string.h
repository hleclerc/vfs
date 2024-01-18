#pragma once

#include <vfs/support/TypeConfig.h>
#include <Python.h>

namespace Vfs {

Str py_string_to_std_string( const PyObject *py_str, bool utf8_only = true );

} // namespace Vfs
