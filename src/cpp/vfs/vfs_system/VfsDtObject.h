#pragma once

// #include "../support/type_promote.h" // IWYU pragma: export
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

    static void          for_each_template_arg    ( auto &&f ) { f( CtInt<inline_data_size>() ); f( CtInt<inline_data_alig>() ); }
    static auto          template_type_name       () { return "VfsDtObject"; }

    TAUV void            construct                ( CtTypeList<A...> /*Ref access*/, CtType<U> /*Object type*/, CtType<V> /*Content type*/, auto &&...ctor_args ) { new ( this ) VfsDtWrap<U,V,A...>( FORWARD( ctor_args )... ); }

    mutable PI32         instantiated_type_index; ///<
    PI32                 global_type_index;       ///<
    char                 data[ data_size ];       ///<
};

///< this function is expected to be surdefined to find which concrete type should be used to construct a VfsObject with FromTypeAndCtorArguments
auto vfs_dt_impl_type( CtType<void> ObjType, const auto &... ) {
    return CtType<void>();
}

/// VfsArgTrait for VfsDtObjects
template<class Obj> requires std::is_base_of_v<VfsDtObject<Obj::_vdo_inline_data_size,Obj::_vdo_inline_data_alig>,Obj>
struct VfsArgTrait<Obj> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const Obj &obj ) {
        VfsDtTypeAncestor *ta = VfsDtTypeAncestor::type_at_global_index( obj.global_type_index );
        ta->get_compilation_flags_rec( cf, seen_for_cf );
        final_types = ta->final_types();
        final_refs = ta->final_refs();
        cast_type = ta->cast_type();
    }

    /// ensures `instantiated_type_index` is correct
    static bool key_update( const Obj &obj ) {
        if ( obj.instantiated_type_index )
            return false;
        VfsDtTypeAncestor *ta = VfsDtTypeAncestor::type_at_global_index( obj.global_type_index );
        obj.instantiated_type_index = ta->get_instantiated_type_index();
        return true;
    }

    static PI32 key_max( const Obj &obj ) {
        const auto &type_table = StaticStorage<VfsDtTypeTable,Obj>::value;
        return obj._vfs_object_rt_data.array_size;
    }

    static PI32 key( const Obj &obj ) {
        return obj.instantiated_type_index;
    }
};

#define VfsDtObject_STD_METHODS( NAME, PATH ) public: \
    static void          get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( PATH "/" #NAME ".h" ); } \
    static auto          type_name            () { return #NAME; } \
    \
    /**/                 NAME                 ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, static_cast<VfsDtObject<_vdo_inline_data_size,_vdo_inline_data_alig> &>( *this ), CtTypeList<VfsRefAccessDirect>(), CtType<NAME>(), FORWARD( ct_type ), FORWARD( args )... ); } \
    /**/                 NAME                 ( FromPointerOnBorrowed, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, static_cast<VfsDtObject<_vdo_inline_data_size,_vdo_inline_data_alig> &>( *this ), CtTypeList<VfsRefAccessDeref<VfsRefAccessDirect,true>>(), CtType<NAME>(), CT_DECAYED_TYPE_OF( pointer ), FORWARD( pointer ) ); } \
    /**/                 NAME                 ( FromPointerOnOwned, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, static_cast<VfsDtObject<_vdo_inline_data_size,_vdo_inline_data_alig> &>( *this ), CtTypeList<VfsRefAccessDeref<VfsRefAccessDirect,false>>(), CtType<NAME>(), CT_DECAYED_TYPE_OF( pointer ), FORWARD( pointer ) ); } \
    /**/                 NAME                 ( FromValue, auto &&value ) : NAME( FromTypeAndCtorArguments(), CT_DECAYED_TYPE_OF( value ), FORWARD( value ) ) {} \
    /**/                 NAME                 ( const NAME &that ) : NAME( FromValue(), that ) {} \
    /**/                 NAME                 ( NAME &&that ) : NAME( FromValue(), std::move( that ) ) {} \
    /**/                 NAME                 ( auto &&...args ) requires requires { vfs_dt_impl_type( CtType<NAME>(), args... ); } : NAME( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<NAME>(), args... ), FORWARD( args )... ) {} \
    \
    /**/                ~NAME                 () { VFS_CALL_CAST_METHOD( destroy, CtStringList<>, void, *this ); } \
    \
    NAME&                operator=            ( const NAME &that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, that ); return *this; } \
    NAME&                operator=            ( NAME &&that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, std::move( that ) ); return *this; } \
    \
    template             <CtStringValue       func> \
    static auto          _real_type_call      ( auto &&...args ) { using Result = VALUE_IN_DECAYED_TYPE_OF( type_promote( CtString<func>(), CT_DECAYED_TYPE_OF( args )... ) ); return vfs_call<func,CtStringList<>,Result>( FORWARD( args )... ); }; \
    \
    DisplayItem*         display              ( auto &ds ) const { return VFS_CALL( display, CtStringList<>, DisplayItem *, ds, *this ); } \

#define VfsDtObject_STD_METHODS_TT( NAME, PATH, TEMPLATE_ARG_0, TEMPLATE_ARG_1 ) \
    static void          for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    VfsDtObject_STD_METHODS( NAME, PATH );


END_VFS_NAMESPACE
