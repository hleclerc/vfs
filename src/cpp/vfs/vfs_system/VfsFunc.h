#pragma once

#include "../support/FuncInfo.h" // IWYU pragma: export
#include "../support/Tuple.h"
#include "Wrapper.h" // IWYU pragma: export

BEG_VFS_INTERNAL_NAMESPACE

/// generic definition
template<CtStringValue name,class CompilationFlags,class OutputIndices,class ReturnType,class... Args>
class VfsFunc {
public:
    using               ArrayKey    = decltype( tuple_cat( wrapper_keys( *(Args *)nullptr )... ) );
    using               Callable    = ReturnType( Args... );
    // using            Array       = VfsFuncArray<Callable,ArrayKey>;

    /**/                VfsFunc     ();

    void                operator()  ( Args ...args );

    TA static Callable* callable_for( const A &...args );
    static void         init        ( Args ...args );

    // Array            array;      ///<
};

template<class ReturnType,class FuncInfo,class... Args>
ReturnType vfs_call( FuncInfo &&func_info, Args&&... args );

END_VFS_INTERNAL_NAMESPACE

/// if args do not generate any ct_key (vfs_object_ct_key), make a direct call. Else, use a vfs_call
#define VFS_CALL_DINK( FUNC, RETURN_TYPE, COMPILATION_FLAGS, PURE_OUTPUT_INDICES, ... ) \
    ( [&]<class Cf,class Oi,class... Args>( const Cf &compilation_flags, const Oi &output_indices, Args&&...args ) { \
        if constexpr ( VFS_INTERNAL_NAMESPACE::HasWrapperKeys<Cf> || VFS_INTERNAL_NAMESPACE::HasWrapperKeys<Oi> || ( VFS_INTERNAL_NAMESPACE::HasWrapperKeys<Args> || ... ) ) \
            return vfs_call<RETURN_TYPE>( FuncInfo<CtString<#FUNC>,Cf,Oi>{ {}, compilation_flags, output_indices }, std::forward<Args>( args )... ); \
        else \
            return FUNC( std::forward<Args>( args )... ); \
    } )( as_ct( COMPILATION_FLAGS ), as_ct( PURE_OUTPUT_INDICES ), __VA_ARGS__ )


#include "VfsFunc.tcc" // IWYU pragma: export
