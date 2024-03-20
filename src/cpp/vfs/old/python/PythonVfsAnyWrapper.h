#pragma once

#include <vfs/Any.h>
#include <Python.h>

BEG_VFS_NAMESPACE

/// python type for PythonVfsFunctionWrapper
extern PyTypeObject pytype_PythonVfsAnyWrapper;

/// python object that stores and handle a vfs function
struct PythonVfsAnyWrapper {
    PyObject_HEAD
    Any obj;
};

END_VFS_NAMESPACE
