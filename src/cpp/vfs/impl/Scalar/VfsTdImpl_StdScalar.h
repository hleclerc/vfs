#pragma once

#include "../../vfs_system/VfsTdImplWithBaseMethods.h"

BEG_VFS_NAMESPACE

///
template<class ScalarWrapper,class ScalarType>
struct VfsTdImpl_StdScalar : VfsTdImplWithBaseMethods<ScalarWrapper,VfsTdImpl_StdScalar<ScalarWrapper,ScalarType>,ScalarType>, WithDefaultOperators {
    using             Vti                  = VfsTdImplWithBaseMethods<ScalarWrapper,VfsTdImpl_StdScalar<ScalarWrapper,ScalarType>,ScalarType>;
    TT struct         WithData             { using value = VfsTdImpl_StdScalar<ScalarWrapper,T>; };

    /**/              VfsTdImpl_StdScalar  ( auto &&...ctor_args ) : Vti( FORWARD( ctor_args )... ) {}

    static void       get_compilation_flags( CompilationFlags &cf ) { cf.add_inc_file( "vfs/impl/Scalar/VfsTdImpl_StdScalar.h" ); }
    static void       for_each_template_arg( auto &&f ) { f( CtType<ScalarWrapper>() ); f( CtType<ScalarType>() ); }
    static auto       template_type_name   () { return "VFS_NAMESPACE::VfsTdImpl_StdScalar"; }
};


END_VFS_NAMESPACE
