#pragma once

#include "../../vfs_system/VfsTdImplWithBaseMethods.h"

BEG_VFS_NAMESPACE

///
template<class IntWrapper,class IntType>
struct VfsTdImpl_StdInt : VfsTdImplWithBaseMethods<IntWrapper,VfsTdImpl_StdInt<IntWrapper,IntType>,IntType>, WithDefaultOperators {
    using             Vti                  = VfsTdImplWithBaseMethods<IntWrapper,VfsTdImpl_StdInt<IntWrapper,IntType>,IntType>;
    TT struct         WithData             { using value = VfsTdImpl_StdInt<IntWrapper,T>; };

    /**/              VfsTdImpl_StdInt     ( auto &&...ctor_args ) : Vti( FORWARD( ctor_args )... ) {}

    static void       get_compilation_flags( CompilationFlags &cf ) { cf.add_inc_file( "vfs/impl/Int/VfsTdImpl_StdInt.h" ); }
    static void       for_each_template_arg( auto &&f ) { f( CtType<IntWrapper>() ); f( CtType<IntType>() ); }
    static auto       template_type_name   () { return "VFS_NAMESPACE::VfsTdImpl_StdInt"; }
};

template<class Int,class IntType>
struct StorageTypeFor<VfsTdImpl_StdInt<Int,IntType>> { using value = IntType; };

END_VFS_NAMESPACE

// #include "../../vfs_system/VfsTdImpl.h"

// BEG_VFS_NAMESPACE

// ///
// template<class Int,class IntType>
// struct VfsTdImpl_StdInt : VfsTdImpl<Int,VfsTdImpl_StdInt<Int,IntType>>, WithDefaultOperators {
//     /**/           VfsTdImpl_StdInt     ( auto &&...ctor_args ) : data( FORWARD( ctor_args )... ) {}

//     TT void        operator=            ( const VfsTdImpl_StdInt<Int,T> &that ) { if constexpr ( requires { data = that.data; } ) data = that; else { data.~IntType(); new ( this ) Int( FromTypeAndCtorArguments(), CtType<VfsTdImpl_StdInt<Int,T>>(), that.data ); } }
//     operator       IntType              () const { return data; }
//     void           set                  ( auto &&that ) { if constexpr ( requires { data = that; } ) data = that; else { data.~IntType(); new ( this ) Int( FORWARD( that ) ); } }

//     static void    get_compilation_flags( CompilationFlags &cf ) { cf.add_inc_file( "vfs/impl/Int/VfsTdImpl_StdInt.h" ); }
//     static void    for_each_template_arg( auto &&f ) { f( CtType<Int>() ); f( CtType<IntType>() ); }
//     static auto    template_type_name   () { return "VFS_NAMESPACE::VfsTdImpl_StdInt"; }

//     const IntType& get_lvalue           () const { return data; }
//     IntType&&      get_rvalue           () { return std::move( data ); }
//     DisplayItem*   display              ( Displayer &ds ) const { return VFS_NAMESPACE::display( ds, data ); }

//     IntType        data;
// };


// END_VFS_NAMESPACE
