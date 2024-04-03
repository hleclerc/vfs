#pragma once

#include "../../vfs_system/TdImpl.h"

BEG_VFS_NAMESPACE
class Int;
END_VFS_NAMESPACE

BEG_VFS_INTERNAL_NAMESPACE

template<class IntType>
struct TdImpl_StdInt : TdImpl<Int,TdImpl_StdInt<IntType>> {
    /**/         TdImpl_StdInt        ( auto &&...ctor_args ) : data( FORWARD( ctor_args )... ) {}

    static void  for_each_template_arg( auto &&f ) { f( CtType<IntType>() ); }
    static auto  template_type_name   () { return "VfsImpl_StdInt"; }

    IntType      data;
};

END_VFS_INTERNAL_NAMESPACE
