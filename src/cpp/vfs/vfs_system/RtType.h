#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

///
class RtType {
public:
    using       Cn       = CompilationFlags;

    static auto type_name() { return "RtType"; }

    Str         name;    ///<
    Cn          cn;      ///<
};

void vfs_object_get_compilation_flags( CompilationFlags &cn, Vec<Str> &seen, const RtType &obj ) {
    cn << obj.cn;
}

const auto &vfs_object_ct_key( const RtType &obj ) {
    return obj.name;
}

Seq<Str> vfs_object_ct_cast( const RtType &obj, bool deref = true ) {
    return { "auto {ARG} = CtType<" + obj.name + ">();" };
}

END_VFS_NAMESPACE
