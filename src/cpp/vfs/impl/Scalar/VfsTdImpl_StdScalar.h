#pragma once

#include "../../vfs_system/VfsTdImpl.h"

BEG_VFS_NAMESPACE

///
template<class Scalar,class ScalarType>
struct VfsTdImpl_StdScalar : VfsTdImpl<Scalar,VfsTdImpl_StdScalar<Scalar,ScalarType>,ScalarType>, WithDefaultOperators {
    using             Parent               = VfsTdImpl<Scalar,VfsTdImpl_StdScalar<Scalar,ScalarType>,ScalarType>;

    /**/              VfsTdImpl_StdScalar  ( auto &&...ctor_args ) : Parent( FORWARD( ctor_args )... ) {}

    TT void           operator=            ( const VfsTdImpl_StdScalar<Scalar,T> &that ) { if constexpr ( requires { data = that.data; } ) data = that; else { data.~ScalarType(); new ( this ) Scalar( FromTypeAndCtorArguments(), CtType<VfsTdImpl_StdScalar<Scalar,T>>(), that.data ); } }
    void              set                  ( auto &&that ) { if constexpr ( requires { data = that; } ) data = that; else { data.~ScalarType(); new ( this ) Scalar( FORWARD( that ) ); } }

    static void       get_compilation_flags( CompilationFlags &cf ) { cf.add_inc_file( "vfs/impl/Scalar/VfsTdImpl_StdScalar.h" ); }
    static void       for_each_template_arg( auto &&f ) { f( CtType<Scalar>() ); f( CtType<ScalarType>() ); }
    static auto       template_type_name   () { return "VFS_NAMESPACE::VfsTdImpl_StdScalar"; }

    const ScalarType& get_lvalue           () const { return data; }
    ScalarType&&      get_rvalue           () { return std::move( data ); }
    DisplayItem*      display              ( Displayer &ds ) const { return VFS_NAMESPACE::display( ds, data ); }
};


END_VFS_NAMESPACE
