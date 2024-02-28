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
    static void      get_compilation_flags( CompilationFlags &cn );
    static Str       type_name            ();
    SI               compare              ( const VirtualArgList &that ) const;

    void             add_borrowed         ( auto *arg ) { add( arg, false ); }
    void             add_owned            ( auto *arg ) { add( arg, true ); }
    void             add                  ( auto *arg, bool owned );

    Vec<Str>         seen_for_cf;
    Vec<Vec<Str>>    final_types;
    Vec<Vec<Str>>    final_refs;
    Vec<Str>         cast_types;
    Vec<Str>         cast_refs;
    Vec<Str>         arg_types;
    Vec<void *>      pointers;
    Vec<Str>         keys;
    CompilationFlags cf;
};

/// VfsArgTrait for VirtualCtString
template<>
struct VfsArgTrait<VirtualArgList> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualArgList &obj );
    static auto key        ( const VirtualArgList &obj ) -> const VirtualArgList&;
};

// implementation
void VirtualArgList::add( auto *arg, bool owned ) {
    using T = DECAYED_TYPE_OF( *arg );
    get_compilation_flags_rec( cf, seen_for_cf, CtType<T>() );

    Vec<Str> &final_type = *final_types.push_back();
    Vec<Str> &final_ref = *final_refs.push_back();
    Str &cast_type = *cast_types.push_back();
    Str &cast_ref = *cast_refs.push_back();
    arg_types << VFS_NAMESPACE::type_name<T>();
    pointers << arg;

    if constexpr ( VfsArg<T> ) {
        VfsArgTrait<T>::get_cg_data( cf, seen_for_cf, cast_type, cast_ref, final_type, final_ref, *arg );
        keys << VfsArgTrait<T>::key( *arg );
    }
}

END_VFS_NAMESPACE

