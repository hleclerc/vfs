#pragma once

#include "../support/CtFuncInfo.h"
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

template<CtStringValue name,CtStringValue... compilations_flags,int... output_indices,class ReturnType,class... Args>
ReturnType vfs_call( CtString<name>, CtStringList<compilations_flags...>, CtIntList<output_indices...>, CtType<ReturnType>, Args&&... args );

END_VFS_INTERNAL_NAMESPACE

/// if args do not generate any ct_key (vfs_object_ct_key), make a direct call. Else, use a vfs_call
#define VFS_CALL_DINK( FUNC, COMPILATION_FLAGS, OUTPUT_INDICES, RETURN_TYPE, ... ) \
    ( [&]<class Cf,class Oi,class Rt,class... Args>( const Cf &compilation_flags, const Oi &output_indices, const Rt &return_type, Args&&...args ) { \
        if constexpr ( VFS_INTERNAL_NAMESPACE::IsAWrapper<Cf> || VFS_INTERNAL_NAMESPACE::IsAWrapper<Oi> || VFS_INTERNAL_NAMESPACE::IsAWrapper<Rt> || ( VFS_INTERNAL_NAMESPACE::IsAWrapper<Args> || ... ) ) \
            return vfs_call( CtFuncInfo<#FUNC>(), compilation_flags, output_indices, return_type, std::forward<Args>( args )... ); \
        else \
            return FUNC( std::forward<Args>( args )... ); \
    } )( COMPILATION_FLAGS, OUTPUT_INDICES, RETURN_TYPE, __VA_ARGS__ )



#include "VfsFunc.tcc" // IWYU pragma: export
