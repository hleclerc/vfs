#pragma once

#include "internal/array_type_for_ctor_args.h"
#include "internal/ArrayTagList.h"
#include "Int.h"

namespace Vfs {

template<class Item,class Tags=ArrayTagList<>>
class Array { VFS_TD_ATTRIBUTES_TT( Array, "vfs", Item, Tags );
public:
    // helpers to allow construction using {} lists
    TT   Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values );
    TT   Array( const std::initializer_list<std::initializer_list<T>> &values );
    TT   Array( const std::initializer_list<T> &values );

    // generic ctor that calls FromTypeAndCtorArguments() with array_type_for_ctor_args as first argument to get the real type
    TA   Array( A &&...args ) requires Has_array_type_for_ctor_args<Item,Tags,A...>;

    Int  size () const;
};

// type info -------------------------------------------------------------------------------------------
TUV auto constexpr tensor_order( CtType<Array<U,V>> ) { return CtInt<1>(); }
TUV auto constexpr item_type( CtType<Array<U,V>> ) { return CtType<U>(); }

}

#include "Array.tcc"
