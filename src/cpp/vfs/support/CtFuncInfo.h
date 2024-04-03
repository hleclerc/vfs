#pragma once

#include "CtStringList.h"
#include "CtIntList.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue str,class ReturnType=void,class PureOutputIndices=CtIntList<>,class CompilationFlags=CtStringList<>>
struct CtFuncInfo {
    static void  for_each_template_arg( auto &&f ) { f( CtString<str.value>() ); f( CtType<ReturnType>() ); f( CtType<PureOutputIndices>() ); f( CtType<CompilationFlags>() ); }
    static auto  template_type_name   () { return "CtFuncInfo"; }
    static auto* display              ( auto &ds ) { return ds.string( str.value ); }
};

END_VFS_NAMESPACE
