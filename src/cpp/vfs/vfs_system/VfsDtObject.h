#pragma once

// #include "../support/type_promote.h" // IWYU pragma: export
#include "VfsArgTrait.h"
#include "VfsDtWrap.h" // IWYU pragma: export
#include "VfsDtType.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

/// `t` is a `VfsDtObject` if there is an attribute `t._vfs_object_rt_data.global_type_index`
TT concept VfsDtObject = requires ( T &t ) {
    t._vfs_object_rt_data.global_type_index;
};


/// VfsArgTrait for VfsDtObject
template<VfsDtObject Obj>
struct VfsArgTrait<Obj,true> {
    static void get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const Obj &obj ) {
        VfsDtTypeAncestor *ta = VfsDtTypeAncestor::type_at_global_index( obj._vfs_object_rt_data.global_type_index );
        ta->get_compilation_flags_rec( cf, seen_for_cf );
        final_types = ta->final_types();
        final_refs = ta->final_refs();
        cast_type = ta->cast_type();
        cast_ref = ta->cast_ref();
    }

    /// ensures `instantiated_type_index` is correct
    static bool key_update( const Obj &obj ) {
        if ( obj._vfs_object_rt_data.instantiated_type_index )
            return false;
        VfsDtTypeAncestor *ta = VfsDtTypeAncestor::type_at_global_index( obj._vfs_object_rt_data.global_type_index );
        obj._vfs_object_rt_data.instantiated_type_index = ta->get_instantiated_type_index();
        return true;
    }

    static PI32 key_max( const Obj &obj ) {
        const auto &type_table = StaticStorage<VfsDtTypeTable,Obj>::value;
        return obj._vfs_object_rt_data.array_size;
    }

    static PI32 key( const Obj &obj ) {
        return obj._vfs_object_rt_data.instantiated_type_index;
    }
};


// helper to forward content of a vfs td object
// auto &&vfs_td_cast( auto type, auto &&obj ) {
//     constexpr bool move = std::is_rvalue_reference_v<decltype( obj )>;
//     if constexpr ( move )
//         return std::move( obj._vfs_object_rt_data.cast( type ) );
//     else
//         return obj._vfs_object_rt_data.cast( type );
// }

///< this function is expected to be surdefined to find which concrete type should be used to construct a VfsObject with FromTypeAndCtorArguments
auto vfs_dt_impl_type( CtType<void> ObjType, const auto &... ) {
    return CtType<void>();
}

#define VFS_DT_OBJECT_ATTRIBUTES( NAME, PATH ) public: \
    static void          get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( PATH "/" #NAME ".h" ); } \
    static auto          template_type_name   () { return #NAME; } \
    \
    using                VfsDtSpec            = VfsDtWrap<NAME>; \
    \
    TT const auto&       impl                 () const { return static_cast<const VfsDtWrap_<VfsDtSpec,T> &>( *this ); } \
    TT auto&             impl                 () { return static_cast<VfsDtWrap_<VfsDtSpec,T> &>( *this ); } \
    \
    /**/                 NAME                 ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_object_rt_data, FromTypeAndCtorArguments(), FORWARD( ct_type ), FORWARD( args )... ); } \
    /**/                 NAME                 ( FromPointer, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_object_rt_data, FromPointer(), FORWARD( pointer ) ); } \
    /**/                 NAME                 ( FromValue, auto &&value ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_object_rt_data, FromValue(), FORWARD( value ) ); } \
    /**/                 NAME                 ( const NAME &that ) { VFS_CALL_METHOD( construct, CtStringList<>, void, _vfs_object_rt_data, FromValue(), that ); } \
    /**/                 NAME                 ( NAME &&that ) { VFS_CALL_METHOD( construct, CtStringList<>, void, _vfs_object_rt_data, FromValue(), std::move( that ) ); } \
    /**/                 NAME                 ( auto &&...args ) requires requires { vfs_dt_impl_type( CtType<NAME>(), args... ); } { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_object_rt_data, FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<NAME>(), args... ), FORWARD( args )... ); } \
    /**/                ~NAME                 () { VFS_CALL( vfs_td_destroy, CtStringList<>, void, *this ); } \
    \
    NAME&                operator=            ( const NAME &that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, that ); return *this; } \
    NAME&                operator=            ( NAME &&that ) { VFS_CALL( vfs_td_reassign, CtStringList<>, void, *this, std::move( that ) ); return *this; } \
    \
    template             <CtStringValue       func> \
    static auto          _real_type_call      ( auto &&...args ) { using Result = VALUE_IN_DECAYED_TYPE_OF( type_promote( CtString<func>(), CT_DECAYED_TYPE_OF( args )... ) ); return vfs_call<func,CtStringList<>,Result>( FORWARD( args )... ); }; \
    \
    DisplayItem*         display              ( auto &ds ) const { return VFS_CALL( display, CtStringList<>, DisplayItem *, ds, *this ); } \
    \
    VfsDtSpec            _vfs_object_rt_data

#define VFS_DT_OBJECT_ATTRIBUTES_TT( NAME, PATH, TEMPLATE_ARG_0, TEMPLATE_ARG_1 ) \
    static void          for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    VFS_DT_OBJECT_ATTRIBUTES( NAME, PATH );


END_VFS_NAMESPACE
