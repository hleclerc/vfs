#pragma once

#include "../support/CompilationFlags.h"
#include "../support/Tuple.h"

BEG_VFS_NAMESPACE

/// base class for objects that wrap "real" data, with a wrapper_key() method to find which is the type.
struct VfsWrapper {};

///
TT auto vfs_wrapper_keys( const T &wrapper ) { if constexpr ( requires { wrapper.wrapper_keys(); } ) return wrapper.wrapper_keys(); else return Tuple<>{}; }

///
TT Str vfs_wrapper_cast_type( const T &wrapper ) { if constexpr ( requires { wrapper.wrapper_cast_type(); } ) return wrapper.wrapper_cast_type(); else return {}; }

//
TT void vfs_wrapper_get_compilation_flags( CompilationFlags &cg, const T &wrapper ) { if constexpr ( requires { wrapper.wrapper_get_compilation_flags( cg ); } ) return wrapper.wrapper_get_compilation_flags( cg ); }

// concept + type test
TT concept HasWrapperKeys = decltype( wrapper_keys( *(const T *)nullptr ) )::size != 0;
TT struct HasWrapperKeys_struct { enum { value = HasWrapperKeys<T> }; };

END_VFS_NAMESPACE
