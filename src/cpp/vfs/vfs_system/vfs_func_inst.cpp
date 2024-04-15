#include "VfsSymbolCache.h"
#include "vfs_func_inst.h"

BEG_VFS_NAMESPACE

void *vfs_func_inst( const Str &name, const CompilationFlags &cf, const Str &return_type, PI nb_args, const Str *arg_types, const Str *cast_types ) {
    Vec<Str> v_cast_types{ FromSizeAndIterator(), nb_args, cast_types };
    Vec<Str> v_arg_types{ FromSizeAndIterator(), nb_args, arg_types };
    return vfs_symbol_cache.find_func( name, cf, return_type, v_arg_types, v_cast_types );
}

ON_INIT {
    // generic forwarders -------------------------------------------------------------------------------------------------------------------------------------------------
    // forward call
    VFS_ADD_SURDEF( "*" ) {
        cg.add_line( "$0$1( $2 );", cg.return_type == "void" ? "" : "return ", cg.func_name, join( cg.forwarded_args() ) );
        return cg.valid( { -1e6 } );
    };

    // forward method call
    VFS_ADD_SURDEF( "*__method" ) {
        cg.add_line( "$0$2.$1( $3 );", cg.return_type == "void" ? "" : "return ", cg.func_name.substr( 0, cg.func_name.size() - 8 ), cg.forwarded_arg( 0 ), join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid( { -1e6 + 1 } );
    };
}

END_VFS_NAMESPACE
