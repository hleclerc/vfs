#pragma once

#include "../containers/EmptyArrayImpl.h"

BEG_VFS_NAMESPACE

// real type of array if constructed from...
auto array_type_for_ctor_args( auto item_type, auto atl, const HasSizeAndAccess auto &v ) {
    if constexpr( requires { v.begin(); } )
        return atl.array_type_for( DECAYED_CT_OF( *v.begin() ), v.size() );
    else
        return atl.array_type_for( DECAYED_CT_OF( v[ 0 ] ), v.size() );
}

// default ArrayType
auto array_type_for_ctor_args( auto item_type, auto atl ) {
    return CtType<EmptyArrayImpl>();
}

// helper concept
template<class Item,class Tags,class...Args>
concept Has_array_type_for_ctor_args = requires ( Tags tags, const Args &...args ) {
    array_type_for_ctor_args( CtType<Item>(), tags, args... );
};

}
