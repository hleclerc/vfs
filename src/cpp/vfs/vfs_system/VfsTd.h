#pragma once

#include "../support/type_promote.h" // IWYU pragma: export
#include "VfsTdType.h" // IWYU pragma: export
#include "VfsFunc.h" // IWYU pragma: export

BEG_VFS_NAMESPACE
class Type;

///
template<class Object_,int size_=3 * sizeof( void * ),int alig_=alignof( void * )>
class alignas( alig_) VfsTd {
public:
    static constexpr int data_size                = size_;
    static constexpr int data_alig                = alig_;
    static constexpr int padd_size                = ( 2 * sizeof( PI32 ) + data_alig - 1 ) / data_alig * data_alig - 2 * sizeof( PI32 );
    using                Object                   = Object_;

    TTA void             construct                ( FromTypeAndCtorArguments, CtType<T>, A &&...ctor_args ); ///< with ( ctor_args... ) (and not {})
    TT void              construct                ( FromPointer, T &&pointer );
    TT void              construct                ( FromValue, T &&value );

    TT void              destroy                  ( CtType<T> );

    TT constexpr bool    not_enough_room_for      ( CtType<T> );
    void* const&         void_ptr                 () const;
    void*&               void_ptr                 ();
    TT const T&          cast                     ( CtType<T> ) const;
    TT T&                cast                     ( CtType<T> );
    TT const T&          cast                     () const;
    TT T&                cast                     ();

    static auto          get_compilation_flags    ( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/VfsTd.h" ); }
    static void          for_each_template_arg    ( auto &&f ) { f( CtType<Object>() ); }
    static auto          template_type_name       () { return "VfsTd"; }

    mutable PI32         instantiated_type_index; ///<
    PI32                 global_type_index;       ///<
    char                 padd[ padd_size ];       ///<
    char                 data[ data_size ];       ///<
};


TT concept HasVfsTd = requires ( T &t ) { t._vfs_type_and_data.global_type_index; };

template<HasVfsTd T> constexpr auto is_virtual( CtType<T> ) { return CtInt<1>(); }

/// get a correct instantiated_type_index if not done
bool vfs_object_update_for_ct_key( const HasVfsTd auto &obj ) {
    if ( obj._vfs_type_and_data.instantiated_type_index )
        return false;
    VfsTdTypeAncestor *ta = VfsTdTypeAncestor::type_at_global_index( obj._vfs_type_and_data.global_type_index );
    obj._vfs_type_and_data.instantiated_type_index = ta->get_instantiated_type_index();
    return true;
}

void vfs_object_get_compilation_flags( CompilationFlags &cn, Vec<Str> &seen, const HasVfsTd auto &obj ) {
    VfsTdTypeAncestor *ta = VfsTdTypeAncestor::type_at_global_index( obj._vfs_type_and_data.global_type_index );
    ta->get_compilation_flags_rec( cn, seen );
}

PI32 vfs_object_ct_key_max( const HasVfsTd auto &obj ) {
    const auto &type_table = StaticStorage<VfsTdTypeTable,std::decay_t<decltype(obj)>>::value;
    return obj._vfs_type_and_data.array_size;
}

PI32 vfs_object_ct_key( const HasVfsTd auto &obj ) {
    return obj._vfs_type_and_data.instantiated_type_index;
}

Vec<Str> vfs_object_ct_cast( const HasVfsTd auto &obj, bool deref ) {
    VfsTdTypeAncestor *ta = VfsTdTypeAncestor::type_at_global_index( obj._vfs_type_and_data.global_type_index );
    return { "auto &&{ARG} = " + Str( deref && ta->is_a_pointer() ? "*" : "" ) + "vfs_td_cast( CtType<" + ta->name() + ">(), FORWARD( {ARG_DECL} ) );" };
}

// helper to forward content of a vfs td object
auto &&vfs_td_cast( auto type, auto &&obj ) {
    constexpr bool move = std::is_rvalue_reference_v<decltype( obj )>;
    if constexpr ( move )
        return std::move( obj._vfs_type_and_data.cast( type ) );
    else
        return obj._vfs_type_and_data.cast( type );
}

///< this function is expected to be surdefined to find which concrete type should be used to construct a VfsObject with FromTypeAndCtorArguments
auto vfs_td_impl_type( CtType<void> ObjType, const auto &... ) {
    return CtType<void>();
}

#define VFS_TD_ATTRIBUTES( NAME, PATH ) public: \
    static void          get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( PATH "/" #NAME ".h" ); } \
    static auto          template_type_name   () { return #NAME; } \
    \
    /**/                 NAME                 ( FromTypeAndCtorArguments, auto &&ct_type, auto &&...args ) { VFS_CALL_METHOD_DINK( construct, CtStringList<>, void, _vfs_type_and_data, FromTypeAndCtorArguments(), FORWARD( ct_type ), FORWARD( args )... ); } \
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

#define VFS_TD_ATTRIBUTES_TT( NAME, PATH, TEMPLATE_ARG_0, TEMPLATE_ARG_1 ) \
    VFS_TD_ATTRIBUTES( NAME, PATH ); \
    static void          for_each_template_arg( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); }

END_VFS_NAMESPACE

#include "VfsTd.tcc" // IWYU pragma: export
