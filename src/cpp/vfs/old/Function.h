#pragma once

#include "vfs_system/VirtualFunction.h"
#include "vfs_system/VfsDtWrap.h"

BEG_VFS_NAMESPACE

class Function { VFS_TD_OBJECT_ATTRIBUTES( Function, "vfs" );
public:
    /**/ Function( StrLike auto &&name, const CompilationFlags &cf ) : Function( FromValue(), VirtualFunction{ .name = name, .cf = cf } ) {}
    /**/ Function( StrLike auto &&name ) : Function( FromValue(), VirtualFunction{ .name = name } ) {}
    //   Function() : Function( CtType<Void>() ) {}

    TT T operator()( CtType<T>, auto &&...args ) const { return VFS_CALL_METHOD( operator(), CtStringList<>, T, FORWARD( args )... ); }
    TT T call      ( auto &&...args ) const { return operator()( CtType<T>(), FORWARD( args )... ); }
};

END_VFS_NAMESPACE
