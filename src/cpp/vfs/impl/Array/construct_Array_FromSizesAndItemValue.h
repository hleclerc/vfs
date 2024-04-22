#pragma once

#include "ArrayImpl.h"

BEG_VFS_NAMESPACE

TT auto construct_Array_FromSizesAndItemValue( T &wrapper, auto &&sizes, auto &&item ) {
    using Sizes = STORAGE_TYPE_OF( sizes );
    using Item = STORAGE_TYPE_OF( item );
    using Res = ArrayImpl<T,Item,Sizes,Sizes>;

    new ( &wrapper ) Res( FromSizesAndItemValue(), FORWARD( sizes ), FORWARD( item ) );
}

END_VFS_NAMESPACE
