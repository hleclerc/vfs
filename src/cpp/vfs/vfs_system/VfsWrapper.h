#pragma once

// #include "VfsObjectTypeFor.h" // IWYU pragma: export
#include "../support/TypeConfig.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

/**
 * Proxy/pointer to symbolic or real reference
*/
class VfsWrapper {
public:
    mutable PI32         instantiated_type_index;
    PI32                 global_type_index;
    void*                data;
};

TT concept IsAVfsWrapper = std::is_base_of_v<VfsWrapper,std::decay_t<T>>;

//     /**/                NAME                 ( FromPointerOnBorrowed, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromPointerOnBorrowed(), FORWARD( pointer ) ); } \
//     /**/                NAME                 ( FromPointerOnOwned, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromPointerOnOwned(), FORWARD( pointer ) ); } \
//     /**/                NAME                 ( FromValue, auto &&value ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromValue(), FORWARD( value ) ); } \
//     /**/                NAME                 ( const NAME &that ) : NAME( FromValue(), that ) {} \
//     /**/                NAME                 ( NAME &&that ) : NAME( FromValue(), std::move( that ) ) {} \
//     \
//     /**/               ~NAME                 () { VFS_CALL_CAST_METHOD( destroy, void, *this ); } \
//     \
//     NAME&               operator=            ( const NAME &that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, that ); return *this; } \
//     NAME&               operator=            ( NAME &&that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, std::move( that ) ); return *this; } \
//     \
//     Type                type                 () const { return VFS_CALL( actual_type_of, CtStringList<>, Type, *this ); } \

#define STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, INCLUDE_PATH, SIZE, ALIG ) public: \
    /**/                   NAME                      ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_wrapper_attributes, CtType<NAME>(), FromTypeAndCtorArguments(), FORWARD( ct_type ), FORWARD( args )... ); } \
    /**/                   NAME                      ( auto &&...args ) requires requires { ( typename VfsObjectTypeFor<NAME,decltype(args)...>::value * )nullptr; } : NAME( FromTypeAndCtorArguments(), CtType<typename VfsObjectTypeFor<NAME,decltype(args)...>::value>(), FORWARD( args )... ) {} \
    \
    static void            get_compilation_flags     ( auto &cn ) { cn.add_inc_file( INCLUDE_PATH "/" #NAME ".h" ); } \
    DisplayItem*           display                   ( auto &ds ) const { return VFS_CALL( display, CtStringList<>, DisplayItem *, ds, *this ); } \
    \
    using                  __WrapperData             = VfsWrapperData<SIZE,ALIG>; \
    __WrapperData          __vfs_wrapper_data;


#define STD_METHODS_FOR_VFS_WRAPPER( NAME, NAMESPACE, PATH, SIZE, ALIG ) \
    static auto            type_name                 () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG );

#define STD_METHODS_FOR_VFS_WRAPPER_TT( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG );

#define STD_METHODS_FOR_VFS_WRAPPER_TA( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); ( f( CtType<TEMPLATE_ARG_1>() ), ... ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG );


END_VFS_NAMESPACE

#include "VfsFunc.h" // IWYU pragma: export
