#pragma once

#include "../support/type_promote.h" // IWYU pragma: export
#include "CtDataOfVfsArgWithRtIntType.h"

BEG_VFS_NAMESPACE

///
TT concept IsAVfsTdObject = requires ( T &t ) { t._vfs_type_and_data.global_type_index; };

///
template<IsAVfsTdObject Arg>
struct VfsArgTrait<Arg,true> {
    /// get a correct instantiated_type_index if not done
    static bool update_of_ct_key( const Arg &obj ) {
        if ( obj._vfs_type_and_data.instantiated_type_index )
            return false;
        VfsTdTypeAncestor *ta = VfsTdTypeAncestor::type_at_global_index( obj._vfs_type_and_data.global_type_index );
        obj._vfs_type_and_data.instantiated_type_index = ta->get_instantiated_type_index();
        return true;
    }

    static void compilation_flags( CompilationFlags &cn, Vec<Str> &seen, const Arg &obj ) {
        VfsTdTypeAncestor *ta = VfsTdTypeAncestor::type_at_global_index( obj._vfs_type_and_data.global_type_index );
        ta->get_compilation_flags_rec( cn, seen );
    }

    static Vec<Str> ct_cast( const Arg &obj, bool deref = true ) {
        VfsTdTypeAncestor *ta = VfsTdTypeAncestor::type_at_global_index( obj._vfs_type_and_data.global_type_index );
        return { "auto &&{ARG} = " + Str( deref * ta->nb_indirections(), '*' ) + "vfs_td_cast( CtType<" + ta->name() + ">(), FORWARD( {ARG_DECL} ) );" };
    }

    static PI32 ct_key_max( const Arg &obj ) {
        const auto &type_table = StaticStorage<VfsTdTypeTable,Arg>::value;
        return obj._vfs_type_and_data.array_size;
    }

    static PI32 ct_key( const Arg &obj ) {
        return obj._vfs_type_and_data.instantiated_type_index;
    }
};


// helper to forward content of a vfs td object
// auto &&vfs_td_cast( auto type, auto &&obj ) {
//     constexpr bool move = std::is_rvalue_reference_v<decltype( obj )>;
//     if constexpr ( move )
//         return std::move( obj._vfs_type_and_data.cast( type ) );
//     else
//         return obj._vfs_type_and_data.cast( type );
// }

///< this function is expected to be surdefined to find which concrete type should be used to construct a VfsObject with FromTypeAndCtorArguments
auto vfs_td_impl_type( CtType<void> ObjType, const auto &... ) {
    return CtType<void>();
}

#define VFS_TD_OBJECT_ATTRIBUTES( NAME, PATH ) public: \
    static void          get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( PATH "/" #NAME ".h" ); } \
    static auto          template_type_name   () { return #NAME; } \
    \
    using                VfsTdSpec            = VfsTd<NAME>; \
    \
    TT const auto&       impl                 () const { return static_cast<const VfsTdImpl<NAME,VfsTdSpec::data_size,VfsTdSpec::data_alig,T> &>( *this ); } \
    TT auto&             impl                 () { return static_cast<VfsTdImpl<NAME,VfsTdSpec::data_size,VfsTdSpec::data_alig,T> &>( *this ); } \
    \
    /**/                 NAME                 ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_type_and_data, FromTypeAndCtorArguments(), FORWARD( ct_type ), FORWARD( args )... ); } \
    /**/                 NAME                 ( FromPointer, auto &&pointer ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_type_and_data, FromPointer(), FORWARD( pointer ) ); } \
    /**/                 NAME                 ( FromValue, auto &&value ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_type_and_data, FromValue(), FORWARD( value ) ); } \
    /**/                 NAME                 ( const NAME &that ) { VFS_CALL_METHOD( construct, CtStringList<>, void, _vfs_type_and_data, FromValue(), that ); } \
    /**/                 NAME                 ( NAME &&that ) { VFS_CALL_METHOD( construct, CtStringList<>, void, _vfs_type_and_data, FromValue(), std::move( that ) ); } \
    /**/                 NAME                 ( auto &&...args ) requires requires { vfs_td_impl_type( CtType<NAME>(), args... ); } { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_type_and_data, FromTypeAndCtorArguments(), vfs_td_impl_type( CtType<NAME>(), args... ), FORWARD( args )... ); } \
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
    VfsTd<NAME>          _vfs_type_and_data

#define VFS_TD_OBJECT_ATTRIBUTES_TT( NAME, PATH, TEMPLATE_ARG_0, TEMPLATE_ARG_1 ) \
    VFS_TD_OBJECT_ATTRIBUTES( NAME, PATH ); \
    static void          for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); }


END_VFS_NAMESPACE
