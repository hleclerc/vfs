#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

///
class RtInt {
public:
    static void get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/RtInt.h" ); }
    static auto type_name            () { return "RtInt"; }

    int         value;
};

inline const auto &vfs_object_ct_key( const RtInt &obj ) {
    return obj.value;
}

inline Vec<Str> vfs_object_ct_cast( const RtInt &obj ) {
    return { "auto {ARG} = CtInt<" + std::to_string( obj.value ) + ">();" };
}

END_VFS_NAMESPACE
