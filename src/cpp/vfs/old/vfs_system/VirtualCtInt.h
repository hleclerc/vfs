#pragma once

#include "../support/CompilationFlags.h"
#include "VfsArgTrait.h"

BEG_VFS_NAMESPACE

///
class VirtualCtInt {
public:
    static void get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/VirtualCtInt.h" ); }
    static auto type_name            () { return "VirtualCtInt"; }

    int         value;
};

/// VfsArgTrait for VirtualCtInt
template<>
struct VfsArgTrait<VirtualCtInt> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtInt &obj ) {
        Str type = "CtInt<" + std::to_string( obj.value ) + ">";
        final_refs = { type + "();" };
        final_types = { type };
    }

    static PI32 key( const VirtualCtInt &obj ) {
        return obj.value;
    }
};

inline VirtualCtInt make_ct_value( int i ) { return { i }; }
inline VirtualCtInt make_ct_value( PI i ) { return { int( i ) }; }

END_VFS_NAMESPACE
