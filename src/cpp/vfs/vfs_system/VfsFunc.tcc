#pragma once

#include "VfsFunc.h"

BEG_VFS_INTERNAL_NAMESPACE

#define DTP template<CtStringValue name,class CompilationFlags,class OutputIndices,class... Args>
#define UTP VfsFunc<name,CompilationFlags,OutputIndices,Args...>



#undef DTP
#undef UTP

template<CtStringValue name,CtStringValue... compilations_flags,int... output_indices,class ReturnType,class... Args>
ReturnType vfs_call( CtString<name>, CtStringList<compilations_flags...>, CtIntList<output_indices...>, CtType<ReturnType>, Args&&... args ) {
    TODO;
    return {};
}

END_VFS_INTERNAL_NAMESPACE
