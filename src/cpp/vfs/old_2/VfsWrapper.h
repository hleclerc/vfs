#pragma once

// #include "VfsObjectTypeFor.h" // IWYU pragma: export
#include "../support/operators/ceil.h" // IWYU pragma: export
#include "../support/CtFuncInfo.h" // IWYU pragma: export
#include "../support/call_new.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

/**
 * Proxy/pointer to symbolic or real reference
 *
 * Base class for Int, Type, Scalar, Array, ...
*/
template<int size,int alig>
class alignas( alig ) VfsWrapper {
public:
    static constexpr int data_attr_size = ceil( 2 * sizeof( PI32 ), alig ) + size - 2 * sizeof( PI32 );
    static constexpr int vfs_data_size  = size;
    static constexpr int vfs_data_alig  = alig;

    mutable PI32         instantiated_type_index; ///< index in _used_ pointer types (i.e. that have been used at least once in a function call). Wrapper specific (there's a table for Int, another one the Scalar, etc...).
    PI32                 global_type_index; ///<  index of ptr type, amongst all the existing pointer types. NOT wrapper specific.
    char                 data[ data_attr_size ];
};

// concept + type test
TT concept IsAVfsWrapper = std::is_base_of_v<VfsWrapper<T::vfs_data_size,T::vfs_data_alig>,std::decay_t<T>>;
TT struct IsAVfsWrapper_struct { enum { value = IsAVfsWrapper<T> }; };

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
//                         NAME                 ( auto &&...args ) requires requires { ( typename VfsObjectTypeFor<NAME,decltype(args)...>::value * )nullptr; } : NAME( FromTypeAndCtorArguments(), CtType<typename VfsObjectTypeFor<NAME,decltype(args)...>::value>(), FORWARD( args )... ) {} \

#define STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, INCLUDE_PATH ) public: \
    /**/                   NAME                      ( FromTypeAndCtorArguments, auto &&type, auto &&...args ) { VFS_CALL_DINK( call_new, CtStringList<>(), CtIntList<0>(), CtType<void>(), *this, FORWARD( type ), FORWARD( args )... ); } \
    \
    static void            get_compilation_flags     ( auto &cn ) { cn.add_inc_file( INCLUDE_PATH "/" #NAME ".h" ); } \
    DisplayItem*           display                   ( auto &ds ) const { return VFS_CALL( CtFuncInfo<"display">(), CtStringList<>, DisplayItem *, ds, *this ); } \


#define STD_METHODS_FOR_VFS_WRAPPER( NAME, NAMESPACE, PATH) \
    static auto            type_name                 () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, PATH )

#define STD_METHODS_FOR_VFS_WRAPPER_TT( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG )

#define STD_METHODS_FOR_VFS_WRAPPER_TA( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); ( f( CtType<TEMPLATE_ARG_1>() ), ... ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_WRAPPER__BASE( NAME, PATH, SIZE, ALIG )


END_VFS_NAMESPACE

#include "VfsFunc.h" // IWYU pragma: export
