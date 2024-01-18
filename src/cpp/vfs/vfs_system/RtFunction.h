#pragma once

#include "../support/CompilationFlags.h"

namespace Vfs {

///
class RtFunction {
public:
    using       Cf                   = CompilationFlags;

    static void get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/RtFunction.h" ); }
    static auto type_name            () { return "RtFunction"; }

    Str         name;
    Cf          cf;
};

void vfs_object_get_compilation_flags( CompilationFlags &cn, Vec<Str> &seen, const RtFunction &obj );
const Str &vfs_object_ct_key( const RtFunction &obj );
Vec<Str> vfs_object_ct_cast( const RtFunction &obj );

} // namespace Vfs
