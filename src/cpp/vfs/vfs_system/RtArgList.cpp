#include "RtArgList.h"
#include <regex>

BEG_VFS_NAMESPACE
    
    void RtArgList::add( void *arg, bool owned, const Str &type_name, const Str &ct_key, const Vec<Str> *lcasts ) {
    // type names
    type_names.push_back( type_name );

    // compilation flags
    if ( owned )
        cf.add_inc_file( "vfs/support/RaiiDelete.h" );

    // casts. TODO FOR OPTIMIZATION: do the string assembly in vfs_object_ct_cast
    Str n = std::to_string( pointers.size() );
    if ( lcasts ) {
        for( PI i = 0; i < lcasts->size(); ++i ) {
            if ( owned ) {
                Str cast = std::regex_replace( lcasts->operator[]( i ), std::regex( "\\{ARG_DECL\\}" ), "std::move( *reinterpret_cast<" + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) )" );
                casts.push_back( cast + " RaiiDelete __raii_del_{ARG}( reinterpret_cast<" + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) );" );
            } else {
                Str cast = std::regex_replace( lcasts->operator[]( i ), std::regex( "\\{ARG_DECL\\}" ), "*reinterpret_cast<" + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] )" );
                casts.push_back( cast );
            }
        }
    } else {
        if ( owned )
            casts.push_back( Str( "auto &&{ARG} = std::move( *reinterpret_cast<" ) + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) ); RaiiDelete __raii_del_{ARG}( &{ARG} );" );
        else
            casts.push_back( Str( "auto &&{ARG} = *reinterpret_cast<" ) + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] );" );
    }

    // keys
    keys += std::to_string( type_name.size() ) + "_" + type_name;
    keys += std::to_string( ct_key.size() ) + "_" + ct_key;
    keys += owned ? "_O" : "_B";

    // pointers
    pointers.push_back( arg );
}

void RtArgList::get_compilation_flags(CompilationFlags &cn) {
    cn.add_inc_file( "vfs/vfs_system/RtArgList.h" );
}

void vfs_object_get_compilation_flags(CompilationFlags &cf, Vec<Str> &seen, const RtArgList &ral) {
    cf << ral.cf;
}

const Str &vfs_object_ct_key(const RtArgList &ral) {
    return ral.keys;
}

const Vec<Str> &vfs_object_ct_cast(const RtArgList &ral) {
    return ral.casts;
}

PI vfs_object_nb_args(const RtArgList &ral) {
    return ral.pointers.size();
}

END_VFS_NAMESPACE
