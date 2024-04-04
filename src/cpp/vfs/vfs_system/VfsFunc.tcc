#pragma once

#include "VfsFunc.h"

BEG_VFS_INTERNAL_NAMESPACE

#define DTP template<CtStringValue name,class CompilationFlags,class OutputIndices,class... Args>
#define UTP VfsFunc<name,CompilationFlags,OutputIndices,Args...>



#undef DTP
#undef UTP

template<class ReturnType,class FuncInfo,class... Args>
ReturnType vfs_call( FuncInfo &&func_info, Args&&... args ) {
    TODO;
    return {};
}

END_VFS_INTERNAL_NAMESPACE
