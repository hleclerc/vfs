#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

/// if name ends with __method, call is made with args[ 0 ].func( args[ 1... ] )
class VirtualFunction {
public:
    using       Cf                   = CompilationFlags;

    static void get_compilation_flags( CompilationFlags &cn );
    static Str  type_name            ();
    bool        operator<            ( const VirtualFunction &that ) const;

    Str         name;
    Cf          cf;
};

/// VfsArgTrait for VirtualCtType
template<>
struct VfsArgTrait<VirtualFunction> {
    static void  get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualFunction &obj );
    static auto& key        ( const VirtualFunction &obj ) { return obj; }
};

END_VFS_NAMESPACE
