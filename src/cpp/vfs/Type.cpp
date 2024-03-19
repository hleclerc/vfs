#include "vfs_system/VfsSymbolCache.h"
#include "vfs_system/VirtualCtString.h"

BEG_VFS_NAMESPACE

ON_INIT {
    // make a CtType from ct values
    VFS_ADD_SURDEF( "construct_Type", "vfs_td", "name", "compilation_flags" ) {
        Str name = VirtualCtString::final_type_name_to_content( cg.final_types[ 1 ] );

        P( cg.final_types[ 2 ] );
        CompilationFlags cn = VirtualCtStringList::final_type_name_to_content( cg.final_types[ 2 ] );
        cn.add_inc_file( "vfs/Type.h" );
        cn.add_flags_to( cg );

        if ( cg.final_types.size() > 3 ) {
            Vec<Str> args;
            for( PI i = 3; i < cg.final_names.size(); ++i )
                args.push_back( va_string( "VALUE_IN_DECAYED_TYPE_OF( $0 )", cg.final_names[ i ] ) );
            cg.add_line( "vfs_td.construct( CtType<Type>(), FromValue(), CtType<$1<$2>>() );", cg.return_type, name, join( args, ", " ) );
        } else
            cg.add_line( "vfs_td.construct( CtType<Type>(), FromValue(), CtType<$1>() );", cg.return_type, name );
        return cg.valid();
    };
}

END_VFS_NAMESPACE
