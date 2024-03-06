#include <vfs/vfs_system/VfsSymbolCache.h>
#include "vfs_to_python_function_map.h"

BEG_VFS_NAMESPACE

std::map<Str,Str> vfs_to_python_function_map;

ON_INIT {
    for( auto t : { "FP32", "FP64", "PI8", "PI16", "PI32", "PI64", "SI8", "SI16", "SI32", "SI64", "Str", "Void", "Tuple" } )
        vfs_to_python_function_map[ t ] = "vfs/python/vfs_to_python/vfs_to_python_base.h";
    vfs_to_python_function_map[ "VecImpl" ] = "vfs/python/vfs_to_python/vfs_to_python_array.h";

    // TODO: add a general mechanism for vfs function to add includes for specific input types
    VFS_ADD_SURDEF( "vfs_to_python", "obj" ) {
        cg.add_inc_path( "`python3-config --includes`" );

        Str rt( cg.final_types[ 0 ] );
        auto p = rt.find( "<" );
        if ( p != rt.npos )
            rt = rt.substr( 0, p );

        // by default, return an Any wrapper
        auto iter = vfs_to_python_function_map.find( rt );
        if ( iter == vfs_to_python_function_map.end() ) {
            cg.add_inc_file( "vfs/python/PythonVfsAnyWrapper.h" );

            cg.add_line( "auto *res = PyObject_NEW( PythonVfsAnyWrapper, &pytype_PythonVfsAnyWrapper );" );
            cg.add_line( "new ( &res->obj ) Any( FORWARD( obj ) );" );
            cg.add_line( "Py_IncRef( (PyObject *)res );" );
            cg.add_line( "return (PyObject *)res;" );
            return cg.valid();
        }

        // else, return the specific python object
        cg.add_inc_file( iter->second );
        cg.add_line( "return vfs_to_python( FORWARD( obj ) );" );
        return cg.valid();
    };
}

END_VFS_NAMESPACE
