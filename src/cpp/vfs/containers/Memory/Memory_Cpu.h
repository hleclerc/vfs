#pragma once

#include "../../support/vfs_namespace.h"

BEG_VFS_NAMESPACE

///
struct Memory_Cpu {
    static auto get_compilation_flags( auto &cn ) { cn.add_inc_file( "vfs/containers/Memory/Memory_Cpu.h" ); }
    static auto type_name            () { return "Memory_Cpu"; }
};

END_VFS_NAMESPACE
