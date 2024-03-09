#pragma once

#include "internal/ArrayTagListAnalyzer.h"
#include "Int.h"

BEG_VFS_NAMESPACE

///
template<class Item,class Tags=ArrayTagList<>>
class Array {
public:
    VfsDtObject_STD_METHODS_TT( Array, Item, Tags, "vfs", 3 * sizeof( void * ), alignof( void * ) );

    // additional ctors to allow construction using {} lists
    TT     Array     ( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values );
    TT     Array     ( const std::initializer_list<std::initializer_list<T>> &values );
    TT     Array     ( const std::initializer_list<T> &values );

    Int    size      () const;

    Array& operator<<( const Item &item ) { VFS_CALL_METHOD( operator<<, void, *this, item ); return *this; }
    Array& operator<<( Item &&item ) { VFS_CALL_METHOD( operator<<, void, *this, std::move( item ) ); return *this; }
};

// types for ctors --------------------------------------------------------------------------
TUV auto vfs_dt_impl_type( CtType<Array<U,V>>, const HasSizeAndAccess auto &that );
TUV auto vfs_dt_impl_type( CtType<Array<U,V>> );

// type info -------------------------------------------------------------------------------------------
TUV auto constexpr tensor_order( CtType<Array<U,V>> ) { return ArrayTagListAnalyzer::want_nb_dims( V() ); }
TUV auto constexpr item_type( CtType<Array<U,V>> ) { return CtType<U>(); }

END_VFS_NAMESPACE

#include "Array.tcc"
