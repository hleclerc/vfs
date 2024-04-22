#pragma once

#include "../support/CompilationFlags.h"
#include "../support/CtIntList.h"
#include "../support/Tuple.h"

BEG_VFS_NAMESPACE

/// base class for objects that wrap "real" data, with a wrapper_key() method to find which is the type.
struct VfsWrapper {};

/// keys for a given type. Return a Tuple<>() for the generic case.
TT auto vfs_wrapper_keys( const T &wrapper ) { if constexpr ( requires { wrapper.wrapper_keys(); } ) return wrapper.wrapper_keys(); else return Tuple<>{}; }

///
TT Str vfs_wrapper_cast_type( const T &wrapper ) { if constexpr ( requires { wrapper.wrapper_cast_type(); } ) return wrapper.wrapper_cast_type(); else return {}; }

//
TT void vfs_wrapper_get_compilation_flags( CompilationFlags &cg, const T &wrapper ) { if constexpr ( requires { wrapper.wrapper_get_compilation_flags( cg ); } ) return wrapper.wrapper_get_compilation_flags( cg ); }

// concept + type test
TT concept HasWrapperKeys = decltype( wrapper_keys( *(const T *)nullptr ) )::size != 0;
TT struct HasWrapperKeys_struct { enum { value = HasWrapperKeys<T> }; };

// helper func
Tijs auto vfs_wrapper_tuple_of_keys( CtInt<i> index, CtIntList<j...> output_indices, const auto &head, const auto &...tail ) {
    if constexpr ( ( ( i == j ) || ... ) )
        return vfs_wrapper_tuple_of_keys( CtInt<i+1>(), output_indices, FORWARD( tail )... );
    else
        return tuple_cat( vfs_wrapper_keys( head ), vfs_wrapper_tuple_of_keys( CtInt<i+1>(), output_indices, tail... ) );
}
auto vfs_wrapper_tuple_of_keys( auto index, auto output_indices ) { return Tuple<>(); }

END_VFS_NAMESPACE
