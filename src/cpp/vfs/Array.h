#pragma once

#include "internal/ArrayTagListAnalyzer.h"
#include "Int.h"

BEG_VFS_NAMESPACE

template<class Item,class Tags=ArrayTagList<>>
class Array { VFS_TD_ATTRIBUTES_TT( Array, "vfs", Item, Tags );
public:
    // helpers to allow construction using {} lists
    TT   Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values );
    TT   Array( const std::initializer_list<std::initializer_list<T>> &values );
    TT   Array( const std::initializer_list<T> &values );

    // generic ctor that calls FromTypeAndCtorArguments() with array_type_for_ctor_args as first argument to get the real type
    TA   Array( A &&...args ) requires ArrayTagListAnalyzer::Has_array_type_for_ctor_args<Item,Tags,A...>;

    Int  size () const;
};

// types for ctors --------------------------------------------------------------------------
TUV auto vfs_td_impl_type( CtType<Array<U,V>>, const ScalarLike auto &value ) {
    return CT_DECAYED_TYPE_OF( value );
}

TUV auto vfs_td_impl_type( CtType<Array<U,V>> ) {
    return CtType<EmptyArrayImpl>();
}

// type info -------------------------------------------------------------------------------------------
TUV auto constexpr tensor_order( CtType<Array<U,V>> ) { return CtInt<1>(); }
TUV auto constexpr item_type( CtType<Array<U,V>> ) { return CtType<U>(); }

END_VFS_NAMESPACE

#include "Array.tcc"
