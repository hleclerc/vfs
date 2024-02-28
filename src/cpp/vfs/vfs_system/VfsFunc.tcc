#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/StaticStorage.h"
#include "../support/type_name.h"
#include "../support/OnInit.h"

#include "apply_on_keys_of_vfs_objects.h"
#include "get_vfs_func_inst.h"
#include "VfsArgTrait.h"
#include "VfsFunc.h"

BEG_VFS_NAMESPACE

#define DTP template<CtStringValue name,class Flags,class Return,class... Args>
#define UTP VfsFunc<name,Flags,Return,Args...>

DTP UTP::VfsFunc() : array( init ) {
}

DTP Return UTP::operator()( Args ...args ) {
    Callable *callable = apply_on_keys_of_vfs_objects( [&]( const auto &...keys ) {
        return *array( keys... );
    }, Tuple<>{}, args... );
    return callable( std::forward<Args>( args )... );
}

DTP TA typename UTP::Callable *UTP::callable_for( const A &...args ) {
    // compilation flags with declared types
    Vec<Str> seen;
    CompilationFlags compilation_flags;
    Flags::for_each_string( [&]( auto str ) {
        compilation_flags << str.to_string();
    } );
    get_compilation_flags_rec( compilation_flags, seen, CtType<Return>() );
    ( get_compilation_flags_rec( compilation_flags, seen, CtType<A>() ), ... );

    // codegen data for each arg
    Vec<Vec<Str>> final_types( FromReservationSize(), sizeof...( A ) );
    Vec<Vec<Str>> final_refs( FromReservationSize(), sizeof...( A ) );
    Vec<Str> cast_types( FromReservationSize(), sizeof...( A ) );
    auto get_cg_data_for = [&]( const auto &arg ) {
        Vec<Str> sub_final_types;
        Vec<Str> sub_final_refs;
        Str cast_type;

        using Obj = DECAYED_TYPE_OF( arg );
        if constexpr( VfsArg<Obj> )
            VfsArgTrait<Obj>::get_cg_data( compilation_flags, seen, cast_type, sub_final_types, sub_final_refs, arg );
        else {
            sub_final_types = { type_name<Obj>() };
            sub_final_refs = { "" };
        }

        final_types << sub_final_types;
        final_refs << sub_final_refs;
        cast_types << cast_type;
    };
    ( get_cg_data_for( args ), ... );

    // get the right callable
    return reinterpret_cast<Callable *>( get_vfs_func_inst(
        name.to_string(),
        type_name<Return>(),
        { type_name<Args>()... },
        { ( std::is_trivial_v<std::decay_t<Args>> && sizeof( Args ) <= sizeof( void * ) )... },
        std::move( compilation_flags ),
        std::move( final_types ),
        std::move( final_refs ),
        std::move( cast_types )
    ) );
}

DTP Return UTP::init( Args ...args ) {
    // to get up to date indices and surdefs
    OnInit::update();

    // test and update object
    bool made_key_update = false;
    auto tst_need_update = [&]( const auto &vfs_object ) {
        using Obj = DECAYED_TYPE_OF( vfs_object );
        if constexpr ( requires { VfsArgTrait<Obj>::key_update( vfs_object ); } )
            if ( VfsArgTrait<Obj>::key_update( vfs_object ) )
                made_key_update = true;
    };
    ( tst_need_update( args ), ... );

    // vfs_func_type_index updated => try again
    auto &vfs_func = StaticStorage<VfsFunc>::value;
    if ( made_key_update )
        return vfs_func( std::forward<Args>( args )... );

    // get or make the right callable
    Callable *callable = callable_for( args... );

    // register it
    Callable **ptr = apply_on_keys_of_vfs_objects( [&]( const auto &...keys ) {
        return vfs_func.array( keys... );
    }, Tuple<>{}, args... );

    *ptr = callable;

    // call it
    return callable( std::forward<Args>( args )... );
}

// -------------------------------------------------------------------- functions --------------------------------------------------------------------
template<CtStringValue name,class Flags,class Return,class... Args>
auto vfs_call( Args&&... args ) {
    auto &vfs_func = StaticStorage<VfsFunc<name,Flags,Return,Args&&...>>::value;
    return vfs_func( std::forward<Args>( args )... );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
