#include "python_to_vfs_function_map.h"
#include "py_string_to_std_string.h"
#include "PythonVfsAnyWrapper.h"
#include "NdArrayWrapper.h"

#include <vfs/containers/HeterogeneousArray.h>
#include <vfs/containers/EmptyArrayImpl.h>
#include <vfs/support/push_back_unique.h>
#include <vfs/support/string/va_string.h>
#include <vfs/support/string/join.h>

namespace Vfs {

std::map<Str,VpiConversionFunc *> python_to_vfs_function_map;

// numpy ------------------------------------------------------------------------------------------------------------------------------
TT static void ndarray_conv_with_type( RtArgList &ral, PyObject *arg ) {
    auto *a = reinterpret_cast<PyArrayObject *>( arg );
    switch ( PyArray_NDIM( a ) ) {
    case 0: return ral.add_owned( new NdArrayWrapper<T,CtIntList<>>( a ) );
    case 1: return ral.add_owned( new NdArrayWrapper<T,CtIntList<-1>>( a ) );
    case 2: return ral.add_owned( new NdArrayWrapper<T,CtIntList<-1,-1>>( a ) );
    case 3: return ral.add_owned( new NdArrayWrapper<T,CtIntList<-1,-1,-1>>( a ) );
    case 4: return ral.add_owned( new NdArrayWrapper<T,CtIntList<-1,-1,-1,-1>>( a ) );
    case 5: return ral.add_owned( new NdArrayWrapper<T,CtIntList<-1,-1,-1,-1,-1>>( a ) );
    default: TODO;
    }
}

static void ndarray_conv( RtArgList &ral, PyObject *arg ) {
    auto *obj = reinterpret_cast<PyArrayObject *>( arg );
    switch ( PyArray_DTYPE( obj )->type ) {
    case 'l': return ndarray_conv_with_type<SI64>( ral, arg );
    case 'i': return ndarray_conv_with_type<SI32>( ral, arg );
    case 'd': return ndarray_conv_with_type<FP64>( ral, arg );
    case 'f': return ndarray_conv_with_type<FP32>( ral, arg );
    default:
        PyErr_Format( PyExc_TypeError, "There's no registered python to vfs correspondance for type '%c'", PyArray_DTYPE( obj )->type );
        P( PyArray_DTYPE( obj )->type );
        TODO;
        return;
    }
    // NPY_BOOLLTR = '?',
    // NPY_BYTELTR = 'b',
    // NPY_UBYTELTR = 'B',
    // NPY_SHORTLTR = 'h',
    // NPY_USHORTLTR = 'H',
    // NPY_INTLTR = 'i',
    // NPY_UINTLTR = 'I',
    // NPY_LONGLTR = 'l',
    // NPY_ULONGLTR = 'L',
    // NPY_LONGLONGLTR = 'q',
    // NPY_ULONGLONGLTR = 'Q',
    // NPY_HALFLTR = 'e',
    // NPY_FLOATLTR = 'f',
    // NPY_DOUBLELTR = 'd',
    // NPY_LONGDOUBLELTR = 'g',
    // NPY_CFLOATLTR = 'F',
    // NPY_CDOUBLELTR = 'D',
    // NPY_CLONGDOUBLELTR = 'G',
    // NPY_OBJECTLTR = 'O',
    // NPY_STRINGLTR = 'S',
    // NPY_STRINGLTR2 = 'a',
    // NPY_UNICODELTR = 'U',
    // NPY_VOIDLTR = 'V',
    // NPY_DATETIMELTR = 'M',
    // NPY_TIMEDELTALTR = 'm',
    // NPY_CHARLTR = 'c',
}

// list ------------------------------------------------------------------------------------------------------------------------------
static void list_conv( RtArgList &ral, PyObject *list ) {
    PI n = PyList_Size( list );

    RtArgList sub_ral;
    for( PI i = 0; i < n; ++i ) {
        PyObject *arg = PyList_GetItem( list, i );
        Str type = _PyType_Name( Py_TYPE( arg ) );

        auto iter = python_to_vfs_function_map.find( type );
        if ( iter == python_to_vfs_function_map.end() )
            PyErr_Format( PyExc_TypeError, "There's no registered vfs correspondance for type \"%s\"", type.c_str() );

        iter->second( sub_ral, arg );
    }

    Vec<Str> types;
    for( PI i = 0; i < sub_ral.type_names.size(); ++i )
        push_back_unique( types, sub_ral.type_names[ i ] );
    std::sort( types.begin(), types.end() );

    if ( types.empty() ) {
        static EmptyArrayImpl empty_array;
        ral.add_borrowed( &empty_array );
        return;
    }

    // we assume here that the dependancies and memory layout is going to be the same in the final type
    using ExHA = HeterogeneousArray<CtTypeList<SI64>>;
    ExHA::get_compilation_flags( ral.cf );
    ral.cf << sub_ral.cf;

    Str type = va_string( "HeterogeneousArray<CtTypeList<$0>>", join( types, "," ) );
    auto *ptr = new HeterogeneousArray<CtTypeList<SI64>>;
    for( PI i = 0; i < n; ++i ) {
        ptr->type_numbers.push_back( std::find( types.begin(), types.end(), sub_ral.type_names[ i ] ) - types.begin() );
        ptr->pointers.push_back( sub_ral.pointers[ i ] );
    }

    ral.add( ptr, /*owned*/true, type, "", /*lcasts*/nullptr );
}

// PythonVfsAnyWrapper ---------------------------------------------------------------------------------------------------------------------
static void vfs_any_wrapper_conv( RtArgList &ral, PyObject *arg ) {
    auto *wro = reinterpret_cast<PythonVfsAnyWrapper *>( arg );
    ral.add_borrowed( &wro->obj );
}

// common types ----------------------------------------------------------------------------------------------------------------------
static void float_conv( RtArgList &ral, PyObject *arg ) { return ral.add_owned( new FP64( PyFloat_AsDouble( arg ) ) ); }
static void int_conv  ( RtArgList &ral, PyObject *arg ) { return ral.add_owned( new SI64( PyLong_AsLong( arg ) ) ); }
static void str_conv  ( RtArgList &ral, PyObject *arg ) { return ral.add_owned( new Str ( py_string_to_std_string( arg ) ) ); }


// registering ------------------------------------------------------------------------------------------------------------------------
ON_INIT {
    python_to_vfs_function_map[ "VfsAnyWrapper" ] = vfs_any_wrapper_conv;
    python_to_vfs_function_map[ "ndarray"       ] = ndarray_conv        ;
    python_to_vfs_function_map[ "float"         ] = float_conv          ;
    python_to_vfs_function_map[ "list"          ] = list_conv           ;
    python_to_vfs_function_map[ "int"           ] = int_conv            ;
    python_to_vfs_function_map[ "str"           ] = str_conv            ;
}

} // namespace Vfs
