#pragma once

#include "vfs_system/VfsTd.h"

namespace Vfs {

class Any { VFS_TD_ATTRIBUTES( Any, "vfs" );
public:
    /**/ Any( auto &&value ) : Any( FromValue(), FORWARD( value ) ) {}
    Any() : Any( Void() ) {}
};

}
