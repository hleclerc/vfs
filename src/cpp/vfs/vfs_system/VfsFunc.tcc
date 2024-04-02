#pragma once

// #include "../support/get_compilation_flags_rec.h"
#include "../support/CompilationFlags.h"
#include "../support/StaticStorage.h"
// #include "../support/type_name.h"
#include "../support/OnInit.h"

// #include "apply_on_vfs_objects.h"
// #include "get_vfs_func_inst.h"
#include "VfsTypeAncestor.h"
#include "VfsWrapper.h"
#include "VfsFunc.h"

BEG_VFS_NAMESPACE

#define DTP template<CtStringValue name,class CompilationFlags,class Return,class... Args>
#define UTP VfsFunc<name,CompilationFlags,Return,Args...>

DTP UTP::VfsFunc() : array( &init, CtType<typename Tuple<std::decay_t<Args>...>::template Filtered<VfsWrapper_struct>>() ) {
}

DTP Return UTP::operator()( Args ...args ) {
    const auto get_ptr = [&]( const auto &...vfs_wrappers ) { return *array( vfs_wrappers... ); };
    Callable *callable = tie( args... ).template filtered_apply<VfsWrapper_struct>( get_ptr );
    return callable( std::forward<Args>( args )... );
}

DTP TA typename UTP::Callable *UTP::callable_for( const A &...args ) {
    // compilation flags
    Vec<Str> seen;
    VFS_NAMESPACE::CompilationFlags compilation_flags;
    CompilationFlags::for_each_string( [&]( auto str ) {
        compilation_flags << str.to_string();
    } );
    get_compilation_flags_rec( compilation_flags, seen, CtType<Return>() );
    ( get_compilation_flags_rec( compilation_flags, seen, CtType<A>() ), ... );

    // codegen data for each arg
    Vec<Str> cast_types( FromReservationSize(), nb_vfs_wrappers );
    tie( args... ).template filtered_apply_seq<VfsWrapper_struct>( [&]( const auto &vfs_wrapper ) {
        VfsTypeAncestor *ta = VfsTypeAncestor::type_at_global_index( vfs_wrapper.__vfs_wrapper_attributes.global_type_index );
        cast_types << ta->cast_type();
    } );

    P( cast_types );
    TODO;

    // Vec<Vec<Str>> final_types( FromReservationSize(), sizeof...( A ) );
    // Vec<Vec<Str>> final_refs( FromReservationSize(), sizeof...( A ) );
    // auto get_cg_data_for = [&]( const auto &arg ) {
    //     Vec<Str> sub_final_types;
    //     Vec<Str> sub_final_refs;
    //     Str cast_type;

    //     using Obj = DECAYED_TYPE_OF( arg );
    //     if constexpr( VfsArg<Obj> )
    //         VfsArgTrait<Obj>::get_cg_data( compilation_flags, seen, cast_type, sub_final_types, sub_final_refs, arg );
    //     else {
    //         sub_final_types = { type_name<Obj>() };
    //         sub_final_refs = { "" };
    //     }

    //     final_types << sub_final_types;
    //     final_refs << sub_final_refs;
    //     cast_types << cast_type;
    // };
    // ( get_cg_data_for( args ), ... );

    // // get the right callable
    // return reinterpret_cast<Callable *>( get_vfs_func_inst(
    //     name.to_string(),
    //     type_name<Return>(),
    //     { type_name<Args &&>()... },
    //     { ( std::is_trivial_v<std::decay_t<Args>> && sizeof( Args ) <= sizeof( void * ) )... },
    //     std::move( compilation_flags ),
    //     std::move( final_types ),
    //     std::move( final_refs ),
    //     std::move( cast_types )
    // ) );
}

DTP Return UTP::init( Args ...args ) {
    // to get up to date indices and surdefs
    OnInit::update();

    // test and update object
    bool made_key_update = false;
    tie( args... ).template filtered_apply_seq<VfsWrapper_struct>( [&]( const auto &vfs_wrapper ) {
        if ( vfs_wrapper.__vfs_wrapper_attributes.instantiated_type_index == 0 ) {
            VfsTypeAncestor *ta = VfsTypeAncestor::type_at_global_index( vfs_wrapper.__vfs_wrapper_attributes.global_type_index );
            vfs_wrapper.__vfs_wrapper_attributes.instantiated_type_index = ta->get_instantiated_type_index();
            made_key_update = true;
        }
    } );

    // vfs_func_type_index updated => try again
    auto &vfs_func = StaticStorage<VfsFunc>::value;
    if ( made_key_update )
        return vfs_func( std::forward<Args>( args )... );

    // get or make the right callable
    Callable *callable = callable_for( args... );

    // register it
    Callable **ptr = tie( args... ).template filtered_apply<VfsWrapper_struct>( [&]( const auto &...vfs_wrappers ) {
        return vfs_func.array( vfs_wrappers... );
    } );

    *ptr = callable;

    // call it
    return callable( std::forward<Args>( args )... );
}

// -------------------------------------------------------------------- functions --------------------------------------------------------------------
// template<CtStringValue name,class CompilationFlags,class Return,class... Args>
// void vfs_call( Args&&... args ) {
//     auto &vfs_func = StaticStorage<VfsFunc<name,CompilationFlags,Return,Args&&...>>::value;
//     return vfs_func( FORWARD( args )... );
// }

template<CtStringValue name,CtStringValue... compilations_flags,int... output_indices,class... Args>
void vfs_call( CtString<name>, CtStringList<compilations_flags...>, CtIntList<output_indices...>, Args&&... args ) {
    TODO;
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
