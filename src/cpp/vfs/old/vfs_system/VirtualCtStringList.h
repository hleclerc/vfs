#pragma once

#include "../containers/Vec.h"
#include "VfsArgTrait.h"
class CompilationFlags;

BEG_VFS_NAMESPACE

///
class VirtualCtStringList {
public:
    static auto final_type_name_to_content( StrView str ) -> Vec<Str>;
    static void get_compilation_flags     ( CompilationFlags &cn );
    static Str  type_name                 ();

    Vec<Str>    value;                    ///<
};

/// VfsArgTrait for VirtualCtString
template<>
struct VfsArgTrait<VirtualCtStringList> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtStringList &obj );
    static Str  key        ( const VirtualCtStringList &obj );
};

END_VFS_NAMESPACE
