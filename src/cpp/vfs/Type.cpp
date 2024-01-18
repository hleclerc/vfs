#include "vfs_system/VfsSymbolCache.h"
#include "vfs_system/RtString.h"

namespace Vfs {

ON_INIT {
    // make a CtType from ct values
    VFS_ADD_SURDEF( "construct_Type", "vfs_td", "name", "compilation_flags" ) {
        Str name = RtString::cast_to_string( cg.arg_casts[ 1 ] );

        CompilationFlags cn = RtStringList::cast_to_string( cg.arg_casts[ 2 ] );
        cn.add_flags_to( cg );

        if ( cg.arg_casts.size() > 3 ) {
            Vec<Str> args;
            for( PI i = 3; i < cg.arg_casts.size(); ++i )
                args.push_back( va_string( "GET_DT_VALUE( arg_$0 )", i ) );
            cg.add_line( "vfs_td.construct( FromValue(), CtType<$0<$1>>() );", name, join( args, ", " ) );
        } else
            cg.add_line( "vfs_td.construct( FromValue(), CtType<$0>() );", name );
        return cg.valid();
    };
}

} // namespace
