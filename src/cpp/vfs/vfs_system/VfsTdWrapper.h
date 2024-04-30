#pragma once

#include "../support/operators/ceil.h" // IWYU pragma: export
#include "../vfs_system/VfsFunc.h" // IWYU pragma: export
#include "../support/construct.h" // IWYU pragma: export
#include "../support/VoidFunc.h" // IWYU pragma: export
#include "VfsTdImplFor.h" // IWYU pragma: export
#include "VfsTdKey.h"

BEG_VFS_NAMESPACE

///
template<class FinalWrapper,int size,int alig>
class alignas( alig ) VfsTdWrapper : public VfsWrapper {
public:
    static constexpr PI    ds                           = ceil( sizeof( VfsTdKey<FinalWrapper> ), alig ) + size - sizeof( VfsTdKey<FinalWrapper> );

    void                   wrapper_get_compilation_flags( CompilationFlags &cg ) const { type_info.get_compilation_flags( cg ); }
    Str                    wrapper_cast_type            () const { return type_info.cast_type(); }
    auto                   wrapper_keys                 () const { return tie( type_info ); }

    VfsTdKey<FinalWrapper> type_info;                   ///<
    char                   data[ ds ];                  ///<
};

// macros ------------------------------------------------------------------------------------------------------------------------------
#define STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, INCLUDE_PATH ) public: \
    /**/                   NAME                      ( FromTypeAndCtorArguments, auto &&type, auto &&...args ) { VFS_CALL_DINK( construct, void, CtStringList<>(), CtIntList<0>(), *this, FORWARD( type ), FORWARD( args )... ); } \
    /**/                   NAME                      ( FromTypeAndCtorArguments, auto &&type, auto &&arg ) { VFS_CALL_DINK( construct, void, CtStringList<>(), CtIntList<0>(), *this, FORWARD( type ), FORWARD( arg ) ); } \
    /**/                   NAME                      ( NoConstruction ) {} \
    /**/                   NAME                      ( auto &&...args ) requires requires { (typename VfsTdImplFor<NAME,decltype(args)...>::value *)nullptr; } : NAME( FromTypeAndCtorArguments(), CtType<typename VFS_NAMESPACE::VfsTdImplFor<NAME,decltype(args)...>::value>(), FORWARD( args )... ) {} \
    /**/                  ~NAME                      () { vfs_call( FuncInfo<CtString<"VFS_NAMESPACE::destroy">,CtStringList<"inc_file:vfs/support/destroy.h">>(), *this ); } \
    \
    NAME&                  operator=                 ( auto &&that ) { vfs_call<void>( FuncInfo<CtString<"reassign_pmt__method">>(), *this, VoidFunc(), FORWARD( that ) ); return *this; } \
    \
    template<class Res> static Res __wrapper_call    ( auto func_name, auto &&...args ) { return vfs_call<Res>( FuncInfo<DECAYED_TYPE_OF(func_name)>(), FORWARD( args )... ); } \
    \
    Type                   impl_type                 () const { Type res{ NoConstruction{} }; vfs_call<void>( FuncInfo<CtString<"impl_type">,CtStringList<"inc_file:vfs/vfs_system/impl_type.h">,CtIntList<0>>(), res, *this ); return res; } \
    \
    static void            get_compilation_flags     ( auto &cn ) { cn.add_inc_file( INCLUDE_PATH "/" #NAME ".h" ); } \
    DisplayItem*           display                   ( auto &ds ) const { return vfs_call<DisplayItem *>( FuncInfo<CtString<"display">>(), ds, *this ); } \


#define STD_METHODS_FOR_VFS_TD_WRAPPER( NAME, NAMESPACE, PATH) \
    static auto            type_name                 () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, PATH )

#define STD_METHODS_FOR_VFS_TD_WRAPPER_TT( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, PATH )

#define STD_METHODS_FOR_VFS_TD_WRAPPER_TA( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); ( f( CtType<TEMPLATE_ARG_1>() ), ... ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, PATH )


END_VFS_NAMESPACE
