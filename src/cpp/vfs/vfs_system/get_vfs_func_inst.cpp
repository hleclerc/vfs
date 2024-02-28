#include "get_vfs_func_inst.h"
#include "VfsSymbolCache.h"

//// nsmake obj_name generic_vfs_surdefs.cpp
#include "generic_vfs_surdefs.h"

BEG_VFS_NAMESPACE
    
void *get_vfs_func_inst(
        const Str &name,
        const Str &return_type,
        Vec<Str> &&arg_types,
        Vec<bool> &&arg_trivs,
        CompilationFlags &&compilation_flags,
        Vec<Vec<Str>> &&final_types,
        Vec<Vec<Str>> &&final_refs,
        Vec<Str> &&cast_types
 ) {
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
    return vfs_symbol_cache.find_func( name, return_type, arg_types, compilation_flags, final_types, final_refs, cast_types );
}

END_VFS_NAMESPACE
