#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/CompilationFlags.h"
#include "../support/string/va_string.h"
#include "../support/type_name.h"
#include "VfsArg.h"

BEG_VFS_NAMESPACE

/// VFS_CALL( ..., VirtualArgList( arg ) )  will make the call with arg[ 0 ], arg[ 1 ], ...
/// onwed pointers => `delete` at the end of the call
class VirtualArgList {
public:
    struct           TypeData             { Vec<Str> final_types, final_refs; Str cast_type, key, ptr_type, ref_type; bool owned; SI compare( const TypeData &that ) const; };
    struct           Key                  { Vec<TypeData> type_data; CompilationFlags cf; SI compare( const Key &that ) const; };

    /**/            ~VirtualArgList       ();

    static void      get_compilation_flags( CompilationFlags &cn );
    static Str       type_name            ();
    SI               compare              ( const VirtualArgList &that ) const;

    void             add_borrowed         ( auto &&ptr ) { add( FORWARD( ptr ), false ); }
    void             add_owned            ( auto &&ptr ) { add( FORWARD( ptr ), true ); }
    void             add                  ( auto &&ptr, bool owned );

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
void VirtualArgList::add( auto &&ptr, bool owned ) {
    static_assert( sizeof( ptr ) == sizeof( void * ) );
    using T = DECAYED_TYPE_OF( *ptr );
    using P = DECAYED_TYPE_OF( ptr );

    Vec<Str> seen_for_cf;
    get_compilation_flags_rec( key.cf, seen_for_cf, CtType<P>() );

    TypeData *ad = key.type_data.push_back();
    ad->ptr_type = VFS_NAMESPACE::type_name<P>();
    ad->ref_type = VFS_NAMESPACE::type_name<T>();
    ad->owned = owned;
    if constexpr ( VfsArg<T> ) {
        VfsArgTrait<T>::get_cg_data( key.cf, seen_for_cf, ad->cast_type, ad->final_types, ad->final_refs, *ptr );
        ad->key = VfsArgTrait<T>::key( *ptr );
    } else {
        ad->final_refs << va_string( "$2*reinterpret_cast<$0 &>( {CAST_NAME}.pointers[ $1 ] )$3", ad->ptr_type, pointers.size(),
            owned ? "{BEG_ARG_FORWARD}" : "",
            owned ? "{END_ARG_FORWARD}" : ""
        );
        ad->final_types << ad->ref_type;
    }

    *reinterpret_cast<P *>( pointers.push_back() ) = FORWARD( ptr );
}

END_VFS_NAMESPACE

