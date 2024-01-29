#pragma once

#include "vfs_system/RtFunction.h"
#include "vfs_system/VfsTd.h"

BEG_VFS_NAMESPACE

class Function { VFS_TD_ATTRIBUTES( Function, "vfs" );
public:
    /**/ Function( StrLike auto &&name, const CompilationFlags &cf ) : Function( FromValue(), RtFunction{ .name = name, .cf = cf } ) {}
    /**/ Function( StrLike auto &&name ) : Function( FromValue(), RtFunction{ .name = name } ) {}
    //   Function() : Function( CtType<Void>() ) {}
};

}
