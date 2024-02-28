#pragma once

#include "../containers/Vec.h"
#include "VfsArgTrait.h"
class CompilationFlags;

BEG_VFS_NAMESPACE

///
class VirtualCtString {
public:
    static Str  final_type_name_to_content( StrView final_type_name );
    static void get_compilation_flags     ( CompilationFlags &cn );
    static Str  type_name                 ();

    Str         value;                    ///<
};

/// VfsArgTrait for VirtualCtString
template<>
struct VfsArgTrait<VirtualCtString> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtString &obj );

    static const Str &key( const VirtualCtString &obj ) {
        return obj.value;
    }
};

END_VFS_NAMESPACE
