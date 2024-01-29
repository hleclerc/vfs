#include "py_string_to_std_string.h"

BEG_VFS_NAMESPACE

Str py_string_to_std_string( const PyObject *py_str, bool utf8_only ) {
    if ( PyBytes_Check( py_str ) )
        return std::string( PyBytes_AS_STRING( py_str ) );

    if ( PyByteArray_Check( py_str ) )
        return std::string( PyByteArray_AS_STRING( py_str ) );

    // Must be unicode then.
    if ( ! PyUnicode_Check( py_str ) ) {
        PyErr_Format( PyExc_ValueError, "In %s \"py_str\" failed PyUnicode_Check()", __FUNCTION__ );
        return "";
    }

    if (PyUnicode_READY(py_str)) {
        PyErr_Format( PyExc_ValueError, "In %s \"py_str\" failed PyUnicode_READY()", __FUNCTION__ );
        return "";
    }

    if ( utf8_only && PyUnicode_KIND( py_str ) != PyUnicode_1BYTE_KIND) {
        PyErr_Format( PyExc_ValueError, "In %s \"py_str\" not utf-8", __FUNCTION__ );
        return "";
    }

    // Python 3 and its minor versions (they vary)
    #if PY_MAJOR_VERSION >= 3
    return Str( (char*)PyUnicode_1BYTE_DATA( py_str ) );
    #else
    // Nasty cast away constness because PyString_AsString takes non-const in Py2
    return Str( (char*)PyString_AsString( const_cast<PyObject *>( py_str ) ) );
    #endif
}

END_VFS_NAMESPACE
