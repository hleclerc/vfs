#pragma once

#include "../support/FuncInfo.h" // IWYU pragma: export
#include "VfsFuncArray.h" // IWYU pragma: export
#include "VfsWrapper.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

/// generic definition
template<CtStringValue name,class CompilationFlags,class OutputIndices,class ReturnType,class... Args>
class VfsFunc {
public:
    using               TupleOfKeys = decltype( tuple_cat( vfs_wrapper_keys( *(std::decay_t<Args> *)nullptr )... ) )::template Map<StorageTypeFor>::value;
    using               Callable    = ReturnType( Args... );
    using               Array       = VfsFuncArray<Callable,TupleOfKeys>;

    /**/                VfsFunc     ();

    ReturnType          operator()  ( Args ...args );

    TA static Callable* callable_for( const A &...args );
    static ReturnType   init        ( Args ...args );

    Array               array;      ///<
};

template<class ReturnType=void,CtStringValue name,class CompilationFlags,int... pure_output_indices,class... Args>
ReturnType vfs_call( FuncInfo<CtString<name>,CompilationFlags,CtIntList<pure_output_indices...>> &&func_info, Args&&... args );

END_VFS_NAMESPACE

/// if args do not generate any ct_key (vfs_object_ct_key), make a direct call. Else, use a vfs_call
#define VFS_CALL_DINK( FUNC, RETURN_TYPE, COMPILATION_FLAGS, PURE_OUTPUT_INDICES, ... ) \
    ( [&]<class Cf,class Oi,class... Args>( const Cf &compilation_flags, const Oi &output_indices, Args&&...args ) { \
        if constexpr ( VFS_NAMESPACE::HasWrapperKeys<Cf> || VFS_NAMESPACE::HasWrapperKeys<Oi> || ( VFS_NAMESPACE::HasWrapperKeys<Args> || ... ) ) \
            return vfs_call<RETURN_TYPE>( FuncInfo<CtString<#FUNC>,Cf,Oi>{ {}, compilation_flags, output_indices }, std::forward<Args>( args )... ); \
        else \
            return FUNC( std::forward<Args>( args )... ); \
    } )( as_ct( COMPILATION_FLAGS ), as_ct( PURE_OUTPUT_INDICES ), __VA_ARGS__ )


#include "VfsFunc.tcc" // IWYU pragma: export
