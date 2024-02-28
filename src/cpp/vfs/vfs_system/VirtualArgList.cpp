#include "VirtualArgList.h"
//#include <regex>

BEG_VFS_NAMESPACE

void VfsArgTrait<VirtualArgList>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualArgList &obj ) {
    // on pourrait faire un cast pour passer des `Vec<void *> pointers` à un tuple avec des unique_ptrs si c'est owned
    final_types = obj.final_types;
    final_refs = obj.final_refs;
    cf << obj.cf;
}

const VirtualArgList &VfsArgTrait<VirtualArgList>::key( const VirtualArgList &obj ) {
    return obj;
}

// void VirtualArgList::add( void *arg, bool owned, const Str &type_name, const Str &ct_key, const Vec<Str> *lcasts ) {
//     // type names
//     type_names.push_back( type_name );

//     // compilation flags
//     if ( owned )
//         cf.add_inc_file( "vfs/support/RaiiDelete.h" );

//     // casts. TODO FOR OPTIMIZATION: do the string assembly in vfs_object_ct_cast
//     Str n = std::to_string( pointers.size() );
//     if ( lcasts ) {
//         for( PI i = 0; i < lcasts->size(); ++i ) {
//             if ( owned ) {
//                 Str cast = std::regex_replace( lcasts->operator[]( i ), std::regex( "\\{ARG_DECL\\}" ), "std::move( *reinterpret_cast<" + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) )" );
//                 casts.push_back( cast + " RaiiDelete __raii_del_{ARG}( reinterpret_cast<" + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) );" );
//             } else {
//                 Str cast = std::regex_replace( lcasts->operator[]( i ), std::regex( "\\{ARG_DECL\\}" ), "*reinterpret_cast<" + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] )" );
//                 casts.push_back( cast );
//             }
//         }
//     } else {
//         if ( owned )
//             casts.push_back( Str( "auto &&{ARG} = std::move( *reinterpret_cast<" ) + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) ); RaiiDelete __raii_del_{ARG}( &{ARG} );" );
//         else
//             casts.push_back( Str( "auto &&{ARG} = *reinterpret_cast<" ) + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] );" );
//     }

//     // keys
//     keys += std::to_string( type_name.size() ) + "_" + type_name;
//     keys += std::to_string( ct_key.size() ) + "_" + ct_key;
//     keys += owned ? "_O" : "_B";

//     // pointers
//     pointers.push_back( arg );
// }

void VirtualArgList::get_compilation_flags(CompilationFlags &cn) {
    cn.add_inc_file( "vfs/vfs_system/VirtualArgList.h" );
}

Str VirtualArgList::type_name() {
    return "VirtualArgList";
}

SI VirtualArgList::compare( const VirtualArgList &that ) const {
    return compare_chain(
        final_types, that.final_types,
        final_refs, that.final_refs,
        cast_types, that.cast_types,
        cast_refs, that.cast_refs,
        keys, that.keys,
        cf, that.cf
    );
}

END_VFS_NAMESPACE
