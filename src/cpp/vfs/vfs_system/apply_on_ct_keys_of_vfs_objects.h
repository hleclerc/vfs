#pragma once

#include "../support/common_macros.h"
#include "VfsArgTrait.h"
#include <tuple>

BEG_VFS_NAMESPACE

auto apply_on_ct_keys_of_vfs_objects( auto &&func, auto &&tuple_so_far ) {
    return std::apply( FORWARD( func ), FORWARD( tuple_so_far ) );
}

auto apply_on_ct_keys_of_vfs_objects( auto &&func, auto &&tuple_so_far, const auto &head, const auto &...tail ) {
    using Arg = DECAYED_TYPE_OF( head );
    if constexpr( requires { VfsArgTrait<Arg>::ct_key( head ); } )
        return apply_on_ct_keys_of_vfs_objects( FORWARD( func ), std::tuple_cat( FORWARD( tuple_so_far ), std::make_tuple( VfsArgTrait<Arg>::ct_key( head ) ) ), tail... );
    else
        return apply_on_ct_keys_of_vfs_objects( FORWARD( func ), FORWARD( tuple_so_far ), tail... );
}

END_VFS_NAMESPACE
