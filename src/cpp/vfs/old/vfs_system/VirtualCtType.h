#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

///
class VirtualCtType {
public:
    using       Cn                   = CompilationFlags;

    static void get_compilation_flags( CompilationFlags &cn );
    static Str  type_name            ();
    int         compare              ( const VirtualCtType &that ) const;

    Str         name;                ///<
    Cn          cn;                  ///<
};

/// VfsArgTrait for VirtualCtType
template<>
struct VfsArgTrait<VirtualCtType> {
    static void  get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtType &obj );
    static auto& key        ( const VirtualCtType &obj ) { return obj; }
};


END_VFS_NAMESPACE
