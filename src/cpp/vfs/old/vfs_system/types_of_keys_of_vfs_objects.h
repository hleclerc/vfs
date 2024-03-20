#pragma once

#include "../support/TypeConfig.h"
#include "VfsArg.h"

BEG_VFS_NAMESPACE

auto types_of_keys_of_vfs_objects( auto result, auto remaining_types ) {
    if constexpr( remaining_types.size() == 0 )
        return result;
    else {
        using Obj = DECAYED_TYPE_OF( remaining_types.make_ref_on_head() );
        if constexpr( VfsArg<Obj> )
            return types_of_keys_of_vfs_objects( result.with( CT_DECAYED_TYPE_OF( VfsArgTrait<Obj>::key( remaining_types.make_ref_on_head() ) ) ), remaining_types.tail() );
        else
            return types_of_keys_of_vfs_objects( result, remaining_types.tail() );
    }
}

END_VFS_NAMESPACE
