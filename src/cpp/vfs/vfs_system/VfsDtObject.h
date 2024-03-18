#pragma once

// #include "../support/type_promote.h" // IWYU pragma: export
#include "VfsTypeException_Dt.h" // IWYU pragma: export
#include "VfsRefAccessDirect.h" // IWYU pragma: export
#include "VfsRefAccessDeref.h" // IWYU pragma: export
#include "VfsArgTrait.h"
#include "VfsDtWrap.h" // IWYU pragma: export
#include "VfsDtType.h" // IWYU pragma: export
#include "VfsFunc.h"

BEG_VFS_NAMESPACE

/// "Dynamically Typed Object"
/// => store some data to retrive the type + some room for data (that can be inlined, or a pointer if size is not large enough)
template< int inline_data_size = sizeof( void * ), int inline_data_alig = alignof( void * ) >
class alignas( inline_data_alig ) VfsDtObject {
public:
    static constexpr int _vdo_inline_data_size    = inline_data_size;
    static constexpr int _vdo_inline_data_alig    = inline_data_alig;
    static constexpr int data_size                = ( 2 * sizeof( PI32 ) + inline_data_alig - 1 ) / inline_data_alig * inline_data_alig - 2 * sizeof( PI32 ) + inline_data_size;

    static void          get_compilation_flags    ( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/VfsDtObject.h" ); }
    static void          for_each_template_arg    ( auto &&f ) { f( CtInt<inline_data_size>() ); f( CtInt<inline_data_alig>() ); }
    static auto          template_type_name       () { return "VfsDtObject"; }

    TUV void             construct                ( CtType<U> /*Object type*/, FromTypeAndCtorArguments, CtType<V>, auto &&...args ) { new ( this ) VfsDtWrap<U,V,VfsRefAccessDirect>( FORWARD( args )... ); }
    TT void              construct                ( CtType<T> /*Object type*/, FromPointerOnBorrowed, auto &&pointer ) { new ( this ) VfsDtWrap<T,DECAYED_TYPE_OF( pointer ),VfsRefAccessDeref<VfsRefAccessDirect,true>>( FORWARD( pointer ) ); }
    TT void              construct                ( CtType<T> /*Object type*/, FromPointerOnOwned, auto &&pointer ) { new ( this ) VfsDtWrap<T,DECAYED_TYPE_OF( pointer ),VfsRefAccessDeref<VfsRefAccessDirect,false>>( FORWARD( pointer ) ); }
    TT void              construct                ( CtType<T> /*Object type*/, FromValue, auto &&value ) { construct( CtType<T>(), FromTypeAndCtorArguments(), CT_DECAYED_TYPE_OF( value ), FORWARD( value ) ); }

    mutable PI32         instantiated_type_index; ///<
    PI32                 global_type_index;       ///<
    char                 data[ data_size ];       ///<
};

///< this function is expected to be surdefined to find which concrete type should be used to construct a VfsObject with FromTypeAndCtorArguments
auto vfs_dt_impl_type( CtType<void> ObjType, const auto &... ) {
    return CtType<void>();
}

/// VfsArgTrait for VfsDtObjects
template<class Obj> requires requires ( const Obj &obj ) { obj.__vfs_dt_attributes; }
struct VfsArgTrait<Obj> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const Obj &obj ) {
        VfsDtTypeAncestor *ta = VfsDtTypeAncestor::type_at_global_index( obj.__vfs_dt_attributes.global_type_index );
        ta->get_compilation_flags_rec( cf, seen_for_cf );
        final_types = ta->final_types();
        final_refs = ta->final_refs();
        cast_type = ta->cast_type();
    }

    /// ensures `instantiated_type_index` is correct
    static bool key_update( const Obj &obj ) {
        if ( obj.__vfs_dt_attributes.instantiated_type_index )
            return false;
        VfsDtTypeAncestor *ta = VfsDtTypeAncestor::type_at_global_index( obj.__vfs_dt_attributes.global_type_index );
        obj.__vfs_dt_attributes.instantiated_type_index = ta->get_instantiated_type_index();
        return true;
    }

    static PI32 key_max( const Obj &obj ) {
        const auto &type_table = StaticStorage<VfsDtTypeTable,Obj>::value;
        return obj.__vfs_dt_attributes.array_size;
    }

    static PI32 key( const Obj &obj ) {
        return obj.__vfs_dt_attributes.instantiated_type_index;
    }
};

#define VfsDtObject_STD_METHODS__BASE( NAME, INCLUDE_PATH, SIZE, ALIG ) public: \
    static void            get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( INCLUDE_PATH "/" #NAME ".h" ); } \
    TUV using              TypeException        = VfsTypeException_Dt<NAME,U,V>; \
    \
    /**/                   NAME                 ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromTypeAndCtorArguments(), FORWARD( ct_type ), FORWARD( args )... ); } \
    /**/                   NAME                 ( FromPointerOnBorrowed, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromPointerOnBorrowed(), FORWARD( pointer ) ); } \
    /**/                   NAME                 ( FromPointerOnOwned, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromPointerOnOwned(), FORWARD( pointer ) ); } \
    /**/                   NAME                 ( FromValue, auto &&value ) { VFS_CALL_METHOD_DINK( construct, void, __vfs_dt_attributes, CtType<NAME>(), FromValue(), FORWARD( value ) ); } \
    /**/                   NAME                 ( const NAME &that ) : NAME( FromValue(), that ) {} \
    /**/                   NAME                 ( NAME &&that ) : NAME( FromValue(), std::move( that ) ) {} \
    /**/                   NAME                 ( auto &&...args ) requires requires { vfs_dt_impl_type( CtType<NAME>(), args... ); } : NAME( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<NAME>(), args... ), FORWARD( args )... ) {} \
    \
    /**/                  ~NAME                 () { VFS_CALL_CAST_METHOD( destroy, void, *this ); } \
    \
    NAME&                  operator=            ( const NAME &that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, that ); return *this; } \
    NAME&                  operator=            ( NAME &&that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, std::move( that ) ); return *this; } \
    \
    DisplayItem*           display              ( auto &ds ) const { return VFS_CALL( display, CtStringList<>, DisplayItem *, ds, *this ); } \
    Type                   type                 () const { return VFS_CALL( actual_type_of, CtStringList<>, Type, *this ); } \
    \
    static constexpr PI    __vfs_dt_data_size   = SIZE; \
    static constexpr PI    __vfs_dt_data_alig   = ALIG; \
    VfsDtObject<SIZE,ALIG> __vfs_dt_attributes;

#define VfsDtObject_STD_METHODS( NAME, PATH, SIZE, ALIG ) \
    static auto            type_name            () { return "VFS_NAMESPACE::" #NAME; } \
    VfsDtObject_STD_METHODS__BASE( NAME, PATH, SIZE, ALIG );

#define VfsDtObject_STD_METHODS_TT( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    static auto            template_type_name   () { return "VFS_NAMESPACE::" #NAME; } \
    VfsDtObject_STD_METHODS__BASE( NAME, PATH, SIZE, ALIG );

#define VfsDtObject_STD_METHODS_TA( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); ( f( CtType<TEMPLATE_ARG_1>() ), ... ); } \
    static auto            template_type_name   () { return "VFS_NAMESPACE::" #NAME; } \
    VfsDtObject_STD_METHODS__BASE( NAME, PATH, SIZE, ALIG );


END_VFS_NAMESPACE
