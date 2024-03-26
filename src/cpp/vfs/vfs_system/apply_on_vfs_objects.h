#pragma once

#include "../support/Tuple.h"

BEG_VFS_NAMESPACE

///
template<class HeadArgs,class... TailArgs>
auto apply_on_vfs_objects( auto &&func, Tuple<HeadArgs &,TailArgs &...> tuple, auto &...vfs_objects ) {
    return apply_on_vfs_objects( FORWARD( func ), tuple.next, vfs_objects..., *tuple.head );
}

///
auto apply_on_vfs_objects( auto &&func, Tuple<>, const auto &...vfs_objects ) {
    return func( vfs_objects... );
}

END_VFS_NAMESPACE
