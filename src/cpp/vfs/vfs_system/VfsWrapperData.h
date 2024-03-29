#pragma once

#include "../support/operators/ceil.h"
#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

//
template<int size_,int alig_>
struct VfsWrapperData {
    static constexpr int data_size = ceil( 2 * sizeof( PI32 ), alig_ ) - 2 * sizeof( PI32 ) + data_size;

    mutable PI32 instantiated_type_index;
    PI32         global_type_index;
    char         data[ data_size ];
};


END_VFS_NAMESPACE
