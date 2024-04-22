#pragma once

#include "ArrayImpl.h"

BEG_VFS_NAMESPACE

TT auto construct_Array_FromSizeAndIterator( T &wrapper, auto &&size, auto &&iterator ) {
    using Sizes = Tuple<DECAYED_TYPE_OF( size )>;
    using Item = DECAYED_TYPE_OF( *iterator );
    using Res = ArrayImpl<T,Item,Sizes,Sizes>;

    new ( &wrapper ) Res( FromSizesAndIterator(), tie( size ), FORWARD( iterator ) );
}

END_VFS_NAMESPACE
