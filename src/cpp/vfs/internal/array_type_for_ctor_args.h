#pragma once

#include "../containers/EmptyArrayImpl.h"

BEG_VFS_NAMESPACE

// real type of array if constructed from...
auto array_type_for_ctor_args( auto item_type, auto tag_list, const HasSizeAndAccess auto &v ) {
    if constexpr( requires { v.begin(); } )
        return tag_list.array_type_for( item_type, DECAYED_CT_OF( *v.begin() ), v.size() );
    else
        return tag_list.array_type_for( item_type, DECAYED_CT_OF( v[ 0 ] ), v.size() );
}

// default ArrayType
auto array_type_for_ctor_args( auto item_type, auto tag_list ) {
    return CtType<EmptyArrayImpl>();
}


// helper concept to check if there is an array_type_for_ctor_args
template<class Item,class Tags,class...Args>
concept Has_array_type_for_ctor_args = requires ( Tags tags, const Args &...args ) {
    array_type_for_ctor_args( CtType<Item>(), tags, args... );
};

END_VFS_NAMESPACE
