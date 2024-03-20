#pragma once

#include <vfs/containers/VecImpl.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>
#include <Python.h>

BEG_VFS_NAMESPACE

// NPY_BOOL=0,
// NPY_BYTE, NPY_UBYTE,
// NPY_SHORT, NPY_USHORT,
// NPY_LONGLONG, NPY_ULONGLONG,
// NPY_FLOAT, NPY_DOUBLE, NPY_LONGDOUBLE,
// NPY_CFLOAT, NPY_CDOUBLE, NPY_CLONGDOUBLE,
// NPY_OBJECT=17,
// NPY_STRING, NPY_UNICODE,

inline auto PyArray_typenum_for( CtType<Bool> ) { return NPY_BOOL; }

inline auto PyArray_typenum_for( CtType<SI64> ) { return NPY_LONG; }
inline auto PyArray_typenum_for( CtType<SI32> ) { return NPY_INT; }
inline auto PyArray_typenum_for( CtType<SI16> ) { return NPY_SHORT; }
inline auto PyArray_typenum_for( CtType<SI8 > ) { return NPY_BYTE; }

inline auto PyArray_typenum_for( CtType<PI64> ) { return NPY_ULONG; }
inline auto PyArray_typenum_for( CtType<PI32> ) { return NPY_UINT; }
inline auto PyArray_typenum_for( CtType<PI16> ) { return NPY_USHORT; }
inline auto PyArray_typenum_for( CtType<PI8 > ) { return NPY_UBYTE; }

inline auto PyArray_typenum_for( CtType<FP64> ) { return NPY_DOUBLE; }
inline auto PyArray_typenum_for( CtType<FP32> ) { return NPY_FLOAT; }

//
PyObject *vfs_to_python( auto &&obj )
    requires ( VALUE_IN_DECAYED_TYPE_OF( tensor_order( CT_DECAYED_TYPE_OF( obj ) ) ) != 0 ) &&
    requires { PyArray_typenum_for( item_type( CT_DECAYED_TYPE_OF( obj ) ) ); }
{
    if( ! PyArray_API )
        import_array();

    // item type
    auto item = item_type( CT_DECAYED_TYPE_OF( obj ) );
    using Item = VALUE_IN_DECAYED_TYPE_OF( item );

    // data
    void *data = std::malloc( sizeof( Item ) * obj.size_tot() );
    obj.copy_data_to( data );

    // shape
    Py_ssize_t shape = obj.size();

    return PyArray_SimpleNewFromData( 1, &shape, PyArray_typenum_for( item ), data );
}


END_VFS_NAMESPACE
