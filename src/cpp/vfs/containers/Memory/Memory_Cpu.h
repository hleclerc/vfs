#pragma once

#include "../../support/CompilationFlags.h"

namespace Vfs {

///
struct Memory_Cpu {
    static auto get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/Memory/Memory_Cpu.h" ); }
    static auto type_name            () { return "Memory_Cpu"; }
};

} // namespace
