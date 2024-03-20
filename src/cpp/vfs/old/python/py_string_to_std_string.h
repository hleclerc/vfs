#pragma once

#include <vfs/support/TypeConfig.h>
#include <Python.h>

BEG_VFS_NAMESPACE

Str py_string_to_std_string( const PyObject *py_str, bool utf8_only = true );

END_VFS_NAMESPACE
