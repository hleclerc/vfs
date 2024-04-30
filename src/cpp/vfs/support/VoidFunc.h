#pragma once

#include "vfs_namespace.h"

BEG_VFS_NAMESPACE

struct VoidFunc {
    static void get_compilation_flags( auto &cn ) { cn.add_inc_file( "vfs/support/VoidFunc.h" ); }
    static auto type_name            () { return "VFS_NAMESPACE::VoidFunc"; }

    void        operator()           ( auto&&... ) const {}
};

END_VFS_NAMESPACE
