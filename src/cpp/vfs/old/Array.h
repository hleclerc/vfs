#pragma once

#include "internal/ArrayTagListAnalyzer.h"
#include "Int.h"

BEG_VFS_NAMESPACE

///
template<class Item,class... Tags>
class Array {
public:
    VfsDtObject_STD_METHODS_TA( Array, Item, Tags, "vfs", 3 * sizeof( void * ), alignof( void * ) );
    using        Sizes      = Array<Int,ArrayTag::ForceNbDimsTo<1>>;

    // additional ctors to allow construction using {} lists
    TT           Array      ( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values );
    TT           Array      ( const std::initializer_list<std::initializer_list<T>> &values );
    TT           Array      ( const std::initializer_list<T> &values );

    Int          size       () const;

    Array&       operator<< ( const Item &item ) { VFS_CALL_METHOD( push_back_if_possible, void, *this, FORWARD( item ) ); return *this; }
    Array&       operator<< ( Item &&item ) { VFS_CALL_METHOD( push_back_if_possible, void, *this, FORWARD( item ) ); return *this; }

    auto         operator() ( auto &&...indices ); ///< return a temporary SelectArray instance. `this` is taken as a non-owning pointer
    void         set_item   ( auto &&value, auto &&...indices ); ///<
    Item         get_item   ( auto &&...indices ) const ; ///<

    static Array fill       ( const Sizes &sizes, const Item &value );


    static auto  type_select( const auto &...indices );
};

// types for ctors --------------------------------------------------------------------------
TTA auto vfs_dt_impl_type( CtType<Array<T,A...>>, const HasSizeAndAccess auto &that );
TTA auto vfs_dt_impl_type( CtType<Array<T,A...>> );

// type info -------------------------------------------------------------------------------------------
TTA auto constexpr tensor_order( CtType<Array<T,A...>> ) { return ArrayTagListAnalyzer::requested_nb_dims( A()... ); }
TTA auto constexpr item_type( CtType<Array<T,A...>> ) { return CtType<T>(); }

END_VFS_NAMESPACE

#include "Array.tcc"
