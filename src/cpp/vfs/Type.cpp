#include "vfs_system/VfsSymbolCache.h"
#include "vfs_system/VirtualCtString.h"

BEG_VFS_NAMESPACE

ON_INIT {
    // make a CtType from ct values
    VFS_ADD_SURDEF( "construct_Type", "vfs_td", "name", "compilation_flags" ) {
        Str name = VirtualCtString::final_type_name_to_content( cg.final_types[ 1 ] );

        CompilationFlags cn = VirtualCtStringList::final_type_name_to_content( cg.final_types[ 2 ] );
        cn.add_flags_to( cg );

        if ( cg.final_types.size() > 3 ) {
            Vec<Str> args;
            for( PI i = 3; i < cg.final_types.size(); ++i )
                args.push_back( va_string( "VALUE_IN_DECAYED_TYPE_OF( arg_$0 )", i ) );
            cg.add_line( "vfs_td.construct( FromValue(), CtType<$0<$1>>() );", name, join( args, ", " ) );
        } else
            cg.add_line( "vfs_td.construct( FromValue(), CtType<$0>() );", name );
        return cg.valid();
    };
}

END_VFS_NAMESPACE
