#pragma once

#include "internal/ArrayTagListAnalyzer.h"
#include "containers/EmptyArrayImpl.h"
#include "support/item_sample.h"
#include "support/with_sizes.h"
#include "Int.h"

BEG_VFS_NAMESPACE

///
template<class Item,class Tags=ArrayTagList<>>
class Array { VFS_DT_OBJECT_ATTRIBUTES_TT( Array, "vfs", Item, Tags );
public:
    // additional ctors to allow construction using {} lists
    TT   Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values );
    TT   Array( const std::initializer_list<std::initializer_list<T>> &values );
    TT   Array( const std::initializer_list<T> &values );

    Int  size () const;
};

// types for ctors --------------------------------------------------------------------------
TUV auto vfs_dt_impl_type( CtType<Array<U,V>>, const HasSizeAndAccess auto &that ) {
    constexpr auto wanted_nb_dims = ArrayTagListAnalyzer::want_nb_dims( V{} );

    /// If it's empty, we're not going to have sample items...
    if ( that.size() == 0 )
        return Type( "EmptyArrayImpl", "vfs/containers/EmptyArrayImpl.h", wanted_nb_dims );

    // scalar
    auto sub_item_type = vfs_dt_impl_type( CtType<U>(), item_sample( that, wanted_nb_dims ) );
    if ( wanted_nb_dims == 0 )
        return Type( sub_item_type );

    // nb_dims >= 1
    return with_sizes( that, [&]( const auto &sizes ) {
        // runtime size
        if ( wanted_nb_dims == -1 ) {
            // find nb dims in ctor args
            TODO;
            return Type( sub_item_type );
        }

        // vector
        if ( wanted_nb_dims == 1 ) {
            // template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
            // static size
            if ( ArrayTagListAnalyzer::want_ct_size_for_dim( V(), CtInt<0>() ) )
                return Type( "Vec", "inc_file:vfs/containers/Vec.h", sub_item_type, sizes[ 0 ], CtInt<0>(), CtInt<0>(), CtInt<0>() );
            // dynamic size
            return Type( "Vec", "inc_file:vfs/containers/Vec.h", sub_item_type, CtInt<-1>(), CtInt<0>(), CtInt<0>(), CtInt<1>() );
        }

        // size >= 2
        TODO;
        return Type( sub_item_type );
    } );
}

TUV auto vfs_dt_impl_type( CtType<Array<U,V>> ) {
    return CtType<EmptyArrayImpl<1>>();
}

// type info -------------------------------------------------------------------------------------------
TUV auto constexpr tensor_order( CtType<Array<U,V>> ) { return CtInt<1>(); }
TUV auto constexpr item_type( CtType<Array<U,V>> ) { return CtType<U>(); }

END_VFS_NAMESPACE

#include "Array.tcc"
