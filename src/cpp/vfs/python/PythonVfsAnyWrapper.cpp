#include <vfs/support/string/to_string.h>
#include "python_to_vfs_function_map.h"
#include "PythonVfsAnyWrapper.h"

// because we will need surdefs
//// nsmake obj_name vfs_to_python_function_map.cpp
//// nsmake obj_name ../Type.cpp
#include "vfs_to_python_function_map.h"
#include <vfs/Type.h>

BEG_VFS_NAMESPACE

static PyObject *__str__( PythonVfsAnyWrapper *self ) {
    Str str = to_string( self->obj );
    return PyUnicode_FromStringAndSize( str.data(), str.size() );
}

static void __finalize__( PythonVfsAnyWrapper *self ) {
    PyObject *error_type, *error_value, *error_traceback;
    PyErr_Fetch( &error_type, &error_value, &error_traceback );

    self->obj.~Any();

    PyErr_Restore( error_type, error_value, error_traceback );
}

static PyObject *__call__( PythonVfsAnyWrapper *self, PyObject *args, PyObject *kwargs ) {
    // update the maps
    OnInit::update();

    // arguments
    PI nb_args = PyTuple_GET_SIZE( args );
    RtArgList ral;

    for( PI i = 0; i < nb_args; ++i ) {
        // get type correspondance
        PyObject *arg = PyTuple_GetItem( args, i );

        Str type = _PyType_Name( Py_TYPE( arg ) );
        auto iter = python_to_vfs_function_map.find( type );
        if ( iter == python_to_vfs_function_map.end() ) {
            PyErr_Format( PyExc_TypeError, "There's no registered vfs correspondance for python type \"%s\"", type.c_str() );
            return Py_None;
        }

        // add argument
        iter->second( ral, arg );
    }

    // call func, with return data in an `Any` vfs object
    Any ares = VFS_CALL( call, Any, self->obj, ral );

    // conversion to a python object
    return VFS_CALL( vfs_to_python, PyObject *, std::move( ares ) );
}

PyTypeObject pytype_PythonVfsAnyWrapper = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "VfsAnyWrapper",
    .tp_basicsize = sizeof( PythonVfsAnyWrapper ),
    .tp_itemsize  = 0,
    .tp_call      = (ternaryfunc)__call__,
    .tp_str       = (reprfunc)__str__,
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_doc       = PyDoc_STR( "wrapper to Vfs objects" ),
    // .tp_methods= Custom_methods,
    .tp_new       = PyType_GenericNew,
    .tp_finalize  = (destructor)__finalize__,
};

END_VFS_NAMESPACE
