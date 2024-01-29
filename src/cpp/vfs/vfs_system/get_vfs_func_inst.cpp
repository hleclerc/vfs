#include "get_vfs_func_inst.h"
#include "VfsSymbolCache.h"

//// nsmake obj_name generic_vfs_surdefs.cpp
#include "generic_vfs_surdefs.h"

BEG_VFS_NAMESPACE
    
    void *get_vfs_func_inst( const Str &name, const Str &return_type, Seq<Str> &&arg_types, Seq<bool> &&arg_trivs, Seq<Seq<Str>> &&ct_casts, CompilationFlags &&cn ) {
    // normalize arguments
    // for( PI i = 0; i < arg_types.size(); ++i ) {
    //     if ( arg_trivs[ i ] ) {
    //         if ( arg_types[ i ].ends_with( "&&" ) )
    //             arg_types[ i ] = strip( arg_types[ i ].substr( 0, arg_types[ i ].size() - 2 ) );
    //         else if ( arg_types[ i ].starts_with( "const " ) && arg_types[ i ].ends_with( "&" ) )
    //             arg_types[ i ] = strip( arg_types[ i ].substr( 1, arg_types[ i ].size() - 2 ) );
    //     }
    // }

    // find or construct the function
    return vfs_symbol_cache.find_func( name, return_type, arg_types, ct_casts, cn );
}

END_VFS_NAMESPACE
