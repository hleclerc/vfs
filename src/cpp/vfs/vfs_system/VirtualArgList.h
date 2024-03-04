#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/CompilationFlags.h"
#include "../support/type_name.h"
#include "VfsArg.h"

BEG_VFS_NAMESPACE

/// VFS_CALL( ..., VirtualArgList( arg ) )  will make the call with arg[ 0 ], arg[ 1 ], ...
/// onwed pointers => `delete` at the end of the call
class VirtualArgList {
public:
    struct           TypeData             { Vec<Str> final_types, final_refs; Str cast_type, arg_type, key; void *pointer; SI compare( const TypeData &that ) const; };
    struct           Key                  { Vec<TypeData> type_data; CompilationFlags cf; SI compare( const Key &that ) const; };

    /**/            ~VirtualArgList       ();

    static void      get_compilation_flags( CompilationFlags &cn );
    static Str       type_name            ();
    SI               compare              ( const VirtualArgList &that ) const;

    void             add_borrowed         ( auto *arg ) { add( arg, false ); }
    void             add_owned            ( auto *arg ) { add( arg, true ); }
    void             add                  ( auto *arg, bool owned );

    Vec<void*>       pointers;
    Key              key;
};

/// VfsArgTrait for VirtualCtString
template<>
struct VfsArgTrait<VirtualArgList> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualArgList &obj );
    static auto key        ( const VirtualArgList &obj ) -> const VirtualArgList::Key &;
};

// implementation
void VirtualArgList::add( auto *arg, bool owned ) {
    //         if ( owned )
    //             casts.push_back( Str( "auto &&{ARG} = std::move( *reinterpret_cast<" ) + type_name + " *>( {ARG_DECL}.pointers[ " + n + " ] ) ); RaiiDelete __raii_del_{ARG}( &{ARG} );" );
    Vec<Str> seen_for_cf;
    using T = DECAYED_TYPE_OF( *arg );
    get_compilation_flags_rec( key.cf, seen_for_cf, CtType<T>() );

    TypeData *ad = key.type_data.push_back();
    ad->arg_type = VFS_NAMESPACE::type_name<T>();
    ad->pointer = arg;

    if constexpr ( VfsArg<T> ) {
        VfsArgTrait<T>::get_cg_data( key.cf, seen_for_cf, ad->cast_type, ad->final_types, ad->final_refs, *arg );
        ad->key = VfsArgTrait<T>::key( *arg );
    } else {
        ad->final_types << ad->arg_type;
        ad->final_refs << "{CAST_NAME}";
    }
    P( ad->final_types );
}

END_VFS_NAMESPACE

