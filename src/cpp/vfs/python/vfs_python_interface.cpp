#include <vfs/vfs_system/VfsSymbolCache.h>
#include <vfs/vfs_system/VirtualFunction.h>

#include <vfs/support/push_back_unique.h>
#include <vfs/support/ThreadPool.h> //  TODO: place it in the appropriate place

#include "py_string_to_std_string.h"
#include "PythonVfsAnyWrapper.h"

USING_VFS_NAMESPACE

// Module --------------------------------------------------------------------------------------------------------------------------------------------
PyObject *function( PyObject *self, PyObject *args, PyObject *kwargs ) {
    CompilationFlags cf;
    Str name;

    // args
    PI sa = PyTuple_Size( args );
    if ( sa >= 1 )
        name = py_string_to_std_string( PyTuple_GET_ITEM( args, 0 ) );

    if ( sa >= 2 ) {
        PyObject *lst = PyTuple_GET_ITEM( args, 1 );
        PI cs = PyList_GET_SIZE( lst );
        for( PI i = 0; i < cs; ++i )
            cf.flags.push_back( py_string_to_std_string( PyList_GET_ITEM( lst, i ) ) );
        Py_DecRef( lst );
    }

    // kwargs
    if ( kwargs ) {
        PI sk = PyDict_GET_SIZE( kwargs );
        if ( sk ) {
            TODO;
        }
    }

    // check args
    if ( name.empty() )
        return PyErr_Format( PyExc_TypeError, "Please specify at least the name of the function" );

    auto *res = PyObject_NEW( PythonVfsAnyWrapper, &pytype_PythonVfsAnyWrapper );
    new ( &res->obj ) Any( VirtualFunction{ .name = name, .cf = cf } );
    Py_IncRef( (PyObject *)res );
    return (PyObject *)res;
}

PyObject *add_global_cpp_flag( PyObject *self, PyObject *args, PyObject *kwargs ) {
    char *cpp_flag = nullptr;
    static const char *kwlist[] = { "inc_flag", NULL };
    PI sa = PyArg_ParseTupleAndKeywords( args, kwargs, "s", (char **)kwlist, &cpp_flag );
    if ( ! sa )
        return NULL;

    push_back_unique( vfs_symbol_cache.global_cpp_flags, cpp_flag );
    return Py_None;
}

PyMethodDef funcs[] = {
    { "function", (PyCFunction)function, METH_VARARGS | METH_KEYWORDS, "Load a function by using name + optionnal list of include directories." },
    { "add_global_cpp_flag", (PyCFunction)add_global_cpp_flag, METH_VARARGS, "Add a global cpp flag for compilation of generated files." },
    { NULL }
};

PyModuleDef vfs_python_interface_mod = {
    PyModuleDef_HEAD_INIT,
    "vfs_python_interface",
    "Module to compile/load vfs functions using their names + arg conversion and wrapping.",
    -1,
    funcs,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_vfs_python_interface( void ) {
    return PyModule_Create( &vfs_python_interface_mod );
}
