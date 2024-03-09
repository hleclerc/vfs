#pragma once

#include "Int.h"

BEG_VFS_NAMESPACE

class String {
public:
    VfsDtObject_STD_METHODS( String, "vfs", 3 * sizeof( void * ), alignof( void * ) );

    friend String operator+( auto &&a, auto &&b ) { return VFS_CALL( add, CtStringList<>, String, FORWARD( a ), FORWARD( b ) ); }
    Int           size     () const;
    operator      Str      () const;
};

// types for ctors --------------------------------------------------------------------------
auto vfs_dt_impl_type( CtType<String>, const StrLike auto &value ) {
    return CtType<Str>();
}

inline auto vfs_dt_impl_type( CtType<String> ) {
    return CtType<Str>();
}

END_VFS_NAMESPACE
