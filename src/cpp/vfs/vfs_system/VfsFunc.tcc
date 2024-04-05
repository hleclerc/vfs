#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/StaticStorage.h"
#include "../support/OnInit.h"
#include "vfs_func_inst.h"
#include "VfsFunc.h"

BEG_VFS_NAMESPACE

#define DTP template<CtStringValue name,class CompilationFlags,class OutputIndices,class ReturnType,class... Args>
#define UTP VfsFunc<name,CompilationFlags,OutputIndices,ReturnType,Args...>

DTP UTP::VfsFunc() : array( init ) {}

DTP ReturnType UTP::operator()( Args ...args ) {
    Callable *callable = *array( tuple_cat( vfs_wrapper_keys( args )... ) );
    return callable( std::forward<Args>( args )... );
}

DTP TA UTP::Callable *UTP::callable_for( const A &...args ) {
    // to get surdefs
    OnInit::update();

    // compilation flags
    Vec<Str> seen;
    VFS_NAMESPACE::CompilationFlags compilation_flags;
    CompilationFlags::for_each_string( [&]( auto str ) {
        compilation_flags << str.to_string();
    } );
    get_compilation_flags_rec( compilation_flags, seen, CtType<ReturnType>() );
    ( get_compilation_flags_rec( compilation_flags, seen, CtType<A>() ), ... );

    // codegen data for each arg
    Str cast_types[ sizeof...( args ) ];
    Str arg_types[ sizeof...( args ) ];
    PI num_arg = 0;
    auto get_cast_types = [&]<class Arg>( const auto &arg, CtType<Arg> ) {
        vfs_wrapper_get_compilation_flags( compilation_flags, arg );
        cast_types[ num_arg ] = vfs_wrapper_cast_type( arg );
        arg_types[ num_arg ] = type_name<Arg>();
        ++num_arg;
    };
    ( get_cast_types( args, CtType<Args>() ), ... );

    // get the right callable
    return reinterpret_cast<Callable *>( vfs_func_inst(
        name.to_string(),
        compilation_flags,
        type_name<ReturnType>(),
        sizeof...( args ),
        arg_types,
        cast_types
    ) );
}

DTP ReturnType UTP::init( Args ...args ) {
    // find and register
    Callable **ptr = StaticStorage<VfsFunc>::value.array( tuple_cat( vfs_wrapper_keys( args )... ) );
    Callable *callable = callable_for( args... );
    *ptr = callable;

    // call
    return callable( std::forward<Args>( args )... );
}


#undef DTP
#undef UTP

template<class ReturnType,CtStringValue name,class CompilationFlags,int... pure_output_indices,class... Args>
ReturnType vfs_call( FuncInfo<CtString<name>,CompilationFlags,CtIntList<pure_output_indices...>> &&func_info, Args&&... args ) {
    return StaticStorage<VfsFunc<name,CompilationFlags,CtIntList<pure_output_indices...>,ReturnType,Args &&...>>::value( FORWARD( args )... );
}

END_VFS_NAMESPACE
