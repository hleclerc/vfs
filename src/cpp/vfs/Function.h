#pragma once

#include "vfs_system/RtFunction.h"
#include "vfs_system/VfsDtRtData.h"

BEG_VFS_NAMESPACE

class Function { VFS_TD_OBJECT_ATTRIBUTES( Function, "vfs" );
public:
    /**/ Function( StrLike auto &&name, const CompilationFlags &cf ) : Function( FromValue(), RtFunction{ .name = name, .cf = cf } ) {}
    /**/ Function( StrLike auto &&name ) : Function( FromValue(), RtFunction{ .name = name } ) {}
    //   Function() : Function( CtType<Void>() ) {}

    TT T operator()( CtType<T>, auto &&...args ) const { return VFS_CALL_METHOD( operator(), CtStringList<>, T, FORWARD( args )... ); }
    TT T call      ( auto &&...args ) const { return operator()( CtType<T>(), FORWARD( args )... ); }
};

END_VFS_NAMESPACE
