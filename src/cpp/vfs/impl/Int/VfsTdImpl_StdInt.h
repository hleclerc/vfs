#pragma once

#include "../../vfs_system/VfsTdImpl.h"

BEG_VFS_NAMESPACE
class Int;

template<class IntType>
struct VfsTdImpl_StdInt : VfsTdImpl<Int,VfsTdImpl_StdInt<IntType>>, WithDefaultOperators {
    /**/           VfsTdImpl_StdInt        ( auto &&...ctor_args ) : data( FORWARD( ctor_args )... ) {}

    static void    get_compilation_flags( CompilationFlags &cf ) { cf.add_inc_file( "vfs/impl/Int/TdImpl_StdInt.h" ); }
    static void    for_each_template_arg( auto &&f ) { f( CtType<IntType>() ); }
    static auto    template_type_name   () { return "VFS_NAMESPACE::VfsTdImpl_StdInt"; }

    const IntType& get_lvalue           () const { return data; }
    IntType&&      get_rvalue           () { return std::move( data ); }
    DisplayItem*   display              ( Displayer &ds ) const { return VFS_NAMESPACE::display( ds, data ); }

    IntType        data;
};


END_VFS_NAMESPACE
