#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/StaticStorage.h"
#include "../support/type_name.h"
#include "../support/OnInit.h"

#include "apply_on_ct_keys_of_vfs_objects.h"
#include "get_vfs_func_inst.h"
#include "VfsFunc.h"

namespace Vfs {

#define DTP template<CtStringValue name,class Return,class... Args>
#define UTP VfsFunc<name,Return,Args...>

DTP UTP::VfsFunc() : array( init ) {
}

DTP Return UTP::operator()( Args ...args ) {
    Callable *callable = apply_on_ct_keys_of_vfs_objects( [&]( const auto &...keys ) {
        return *array( keys... );
    }, Tuple<>{}, args... );
    return callable( std::forward<Args>( args )... );
}

DTP TA typename UTP::Callable *UTP::callable_for( const A &...args ) {
    // includes with declared types
    Vec<Str> seen;
    CompilationFlags cn;
    get_compilation_flags_rec( cn, seen, CtType<Return>() );
    ( get_compilation_flags_rec( cn, seen, CtType<A>() ), ... );

    // ct casts + nb_args + compilation needs for each arg
    Vec<Vec<Str>> ct_casts;
    ct_casts.reserve( sizeof...( A ) );
    auto get_ct_cast_for = [&]( const auto &arg ) {
        // includes
        if constexpr( requires { vfs_object_get_compilation_flags( cn, seen, arg ); } )
            vfs_object_get_compilation_flags( cn, seen, arg );

        // casts
        if constexpr( requires { vfs_object_ct_cast( arg ); } )
            ct_casts.emplace_back( vfs_object_ct_cast( arg ) );
        else
            ct_casts.push_back( { "" } );
    };
    ( get_ct_cast_for( args ), ... );

    // get the right callable
    return reinterpret_cast<Callable *>( get_vfs_func_inst(
        name.to_string(),
        type_name<Return>(),
        { type_name<Args>()... },
        { ( std::is_trivial_v<std::decay_t<Args>> && sizeof( Args ) <= sizeof( void * ) )... },
        std::move( ct_casts ),
        std::move( cn )
    ) );
}

DTP Return UTP::init( Args ...args ) {
    // to get up to date indices and surdefs
    // VfsTypeAncestor::register_the_new_types();
    OnInit::update();

    // test and update object
    bool made_key_update = false;
    auto tst_need_update = [&]( const auto &vfs_object ) {
        if constexpr ( requires { vfs_object_update_for_ct_key( vfs_object ); } )
            if ( vfs_object_update_for_ct_key( vfs_object ) )
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
    Callable **ptr = apply_on_ct_keys_of_vfs_objects( [&]( const auto &...keys ) {
        return vfs_func.array( keys... );
    }, Tuple<>{}, args... );
    *ptr = callable;

    // call it
    return callable( std::forward<Args>( args )... );
}

// -------------------------------------------------------------------- functions --------------------------------------------------------------------
template<CtStringValue name,class Return,class... Args>
auto vfs_call( Args&&... args ) {
    auto &vfs_func = StaticStorage<VfsFunc<name,Return,Args&&...>>::value;
    return vfs_func( std::forward<Args>( args )... );
}

#undef DTP
#undef UTP

}
