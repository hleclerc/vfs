#pragma once

// #include "../support/type_promote.h" // IWYU pragma: export
#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

// /**/                   NAME                 ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromTypeAndCtorArguments(), FORWARD( ct_type ), FORWARD( args )... ); } \
//     /**/                   NAME                 ( FromPointerOnBorrowed, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromPointerOnBorrowed(), FORWARD( pointer ) ); } \
//     /**/                   NAME                 ( FromPointerOnOwned, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromPointerOnOwned(), FORWARD( pointer ) ); } \
//     /**/                   NAME                 ( FromValue, auto &&value ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromValue(), FORWARD( value ) ); } \
//     /**/                   NAME                 ( const NAME &that ) : NAME( FromValue(), that ) {} \
//     /**/                   NAME                 ( NAME &&that ) : NAME( FromValue(), std::move( that ) ) {} \
//     /**/                   NAME                 ( auto &&...args ) requires requires { vfs_dt_impl_type( CtType<NAME>(), args... ); } : NAME( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<NAME>(), args... ), FORWARD( args )... ) {} \
//     \
//     /**/                  ~NAME                 () { VFS_CALL_CAST_METHOD( destroy, void, *this ); } \
//     \
//     NAME&                  operator=            ( const NAME &that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, that ); return *this; } \
//     NAME&                  operator=            ( NAME &&that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, std::move( that ) ); return *this; } \
//     \
//     DisplayItem*           display              ( auto &ds ) const { return VFS_CALL( display, CtStringList<>, DisplayItem *, ds, *this ); } \
//     Type                   type                 () const { return VFS_CALL( actual_type_of, CtStringList<>, Type, *this ); } \

#define STD_METHODS_VFS_WRAPPER__BASE( NAME, INCLUDE_PATH, SIZE, ALIG ) public: \
    static void            get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( INCLUDE_PATH "/" #NAME ".h" ); } \
    \
    \
    static constexpr PI    __vfs_dt_data_size   = SIZE; \
    static constexpr PI    __vfs_dt_data_alig   = ALIG; \
    VfsDtObject<SIZE,ALIG> __vfs_dt_attributes;

#define STD_METHODS_VFS_WRAPPER( NAME, NAMESPACE, PATH, SIZE, ALIG ) \
    static auto            type_name            () { return NAMESPACE "::" #NAME; } \
    STD_METHODS_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG );

#define STD_METHODS_VFS_WRAPPER_TT( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    static auto            template_type_name   () { return NAMESPACE "::" #NAME; } \
    STD_METHODS_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG );

#define STD_METHODS_VFS_WRAPPER_TA( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); ( f( CtType<TEMPLATE_ARG_1>() ), ... ); } \
    static auto            template_type_name   () { return NAMESPACE "::" #NAME; } \
    STD_METHODS_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG );


END_VFS_NAMESPACE
