#pragma once

#include "CtStringList.h"
#include "CtIntList.h"

BEG_VFS_NAMESPACE

///
template<class Name,class CompilationFlags=CtStringList<>,class PureOutputIndices=CtIntList<>>
struct FuncInfo {
    static void       for_each_template_arg( auto &&f ) { f( CtType<Name>() ); f( CtType<CompilationFlags>() ); f( CtType<PureOutputIndices>() ); }
    static auto       template_type_name   () { return "VFS_NAMESPACE::FuncInfo"; }
    auto*             display              ( auto &ds ) { return VFS_NAMESPACE::display( name ); }

    Name              name;
    CompilationFlags  compilation_flags;
    PureOutputIndices pure_output_indices;
};

END_VFS_NAMESPACE
