#pragma once

#include "containers/EmptyArrayImpl.h"
#include "support/item_sample.h"
#include "support/with_sizes.h"
#include "Array.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item,class Tags>
#define UTP Array<Item,Tags>

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values ) : Array( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<Array>(), values ), values ) {
}

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<T>> &values ) : Array( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<Array>(), values ), values ) {
}

DTP TT UTP::Array( const std::initializer_list<T> &values ) : Array( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<Array>(), values ), values ) {
}

DTP Int UTP::size() const {
    return VFS_CALL_METHOD( size, Int, *this );
}

DTP UTP UTP::fill( const Sizes &sizes, const Item &value ) {
    return VFS_CALL_DINK( make_FilledArray, CtStringList<"inc_file:vfs/containers/FilledArray.h">, UTP, CtType<UTP>{}, sizes, value, sizes.size() );
}

DTP auto vfs_dt_impl_type( CtType<UTP>, const HasSizeAndAccess auto &that ) {
    constexpr auto wanted_nb_dims = ArrayTagListAnalyzer::want_nb_dims( Tags{} );

    /// If it's empty, we're not going to have sample items...
    if ( that.size() == 0 )
        return Type( "EmptyArrayImpl", "vfs/containers/EmptyArrayImpl.h", wanted_nb_dims );

    // scalar
    auto sub_item_type = vfs_dt_impl_type( CtType<Item>(), item_sample( that, wanted_nb_dims ) );
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
            if ( ArrayTagListAnalyzer::want_ct_size_for_dim( Tags{}, CtInt<0>() ) )
                return Type( CtString<"Vec">(), CtStringList<"inc_file:vfs/containers/Vec.h">(), sub_item_type, sizes[ 0 ], CtInt<0>(), CtInt<0>(), CtInt<0>() );
            // dynamic size
            return Type( CtString<"Vec">(), CtStringList<"inc_file:vfs/containers/Vec.h">(), sub_item_type, CtInt<-1>(), CtInt<0>(), CtInt<0>(), CtInt<1>() );
        }

        // size >= 2
        TODO;
        return Type( sub_item_type );
    } );
}

DTP auto vfs_dt_impl_type( CtType<UTP> ) {
    return CtType<EmptyArrayImpl<1>>();
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
