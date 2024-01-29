#pragma once

#include <vfs/vfs_system/VfsFunc.h>
#include "../PyObject.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

PyObject *vfs_to_python( StrLike auto &&s ) { return PyUnicode_FromStringAndSize( s.data(), s.size() ); }

PyObject *vfs_to_python( SI64 v ) { return PyLong_FromLong( v ); }
PyObject *vfs_to_python( SI32 v ) { return PyLong_FromLong( v ); }

PyObject *vfs_to_python( Void ) { return Py_None; }

// TODO: Tuple<A...> &&
TA PyObject *vfs_to_python( const Tuple<A...> &s ) {
    auto *res = PyTuple_New( sizeof...( A ) );
    std::apply( [&]( const auto &...args ) {
        PI n = 0;
        auto set_item = [&]( const auto &arg ) {
            PyObject *obj = VFS_CALL( vfs_to_python, PyObject *, arg );
            PyTuple_SET_ITEM( res, n++, obj );
        };
        ( set_item( args ), ... );
    }, s );
    return res;
}

END_VFS_NAMESPACE
