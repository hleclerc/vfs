#pragma once

#include "../../vfs_system/VfsTdImpl.h"
#include "../../support/Void.h"

BEG_VFS_NAMESPACE

///
template<class Wrapper,class Type>
struct VfsTdImpl_StdType : VfsTdImpl<Wrapper,VfsTdImpl_StdType<Wrapper,Type>,Void>, CtType<Type> {
    /**/           VfsTdImpl_StdType    ( CtType<Type> ) {}

    //TT void      operator=            ( const VfsTdImpl_StdInt<Int,T> &that ) { if constexpr ( requires { data = that.data; } ) data = that; else { data.~IntType(); new ( this ) Int( FromTypeAndCtorArguments(), CtType<VfsTdImpl_StdInt<Int,T>>(), that.data ); } }

    static void    get_compilation_flags( CompilationFlags &cf ) { cf.add_inc_file( "vfs/impl/Type/VfsTdImpl_StdType.h" ); }
    static void    for_each_template_arg( auto &&f ) { f( CtType<Wrapper>() ); f( CtType<Type>() ); }
    static auto    template_type_name   () { return "VFS_NAMESPACE::VfsTdImpl_StdType"; }

    DisplayItem*   display              ( Displayer &ds ) const { return VFS_NAMESPACE::display( ds, type_name<Type>() ); }
};


END_VFS_NAMESPACE
