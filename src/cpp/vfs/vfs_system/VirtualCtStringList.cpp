#include "../support/CompilationFlags.h"
#include "../support/string/ctor_for.h"
#include "../support/string/join.h"
#include "VirtualCtStringList.h"

BEG_VFS_NAMESPACE

Vec<Str> VirtualCtStringList::final_type_name_to_content( StrView str ) {
    Vec<Str> res;
    PI i = str.find( "CtStringList<" ) + 13;
    for( ; i < str.size(); ++i ) {
        if ( str[ i ] == '{' || str[ i ] == '}' || str[ i ] == ' ' || str[ i ] == ',' )
            continue;

        if ( str[ i ] == '"' ) {
            PI b = ++i;
            for( ; i < str.size(); ++i ) {
                if ( str[ i ] == '\\' && i + 1 < str.size() ) {
                    ++i;
                    continue;
                }
                if ( str[ i ] == '"' )
                    break;
            }
            res.push_back( Str{ str.begin() + b, str.begin() + i } );
        }
    }

    return res;
}

void VirtualCtStringList::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/vfs_system/VirtualCtStringList.h" );
}

Str VirtualCtStringList::type_name() {
    return "VirtualCtStringList";
}

void VfsArgTrait<VirtualCtStringList>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtStringList &obj ) {
    final_types = { "CtStringList<" + join_map( obj.value, ctor_for<Str> ) + ">" };
    final_refs = { "auto {FINAL_NAME} = " + final_types[ 0 ] + "();" };
}

Str VfsArgTrait<VirtualCtStringList>::key( const VirtualCtStringList &obj ) {
    return join_map( obj.value, ctor_for<Str> ); // TODO OPTIMIZE (Vec<Str> does not support comparison)
}


END_VFS_NAMESPACE



