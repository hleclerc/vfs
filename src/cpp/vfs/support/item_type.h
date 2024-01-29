#pragma once

#include "../containers/Vec.h"

BEG_VFS_NAMESPACE
    
        TT constexpr auto item_type( CtType<Vec<T>> ) { return CtType<T>(); }

END_VFS_NAMESPACE
