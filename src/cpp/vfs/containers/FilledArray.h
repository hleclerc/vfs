#pragma once

#include "Vec.h"

BEG_VFS_NAMESPACE

///
template<class Item,int dim>
class FilledArray {
public:
    PI          size      () const { return sizes[ 0 ]; }

    const auto& operator[]( PI i ) const {
        if constexpr ( dim > 1 )
            return reinterpret_cast<FilledArray<Item,dim-1> &>( sizes[ 1 ] );
        else
            return value;
    }

    Vec<PI,dim> sizes;
    Item        value;
};

template<class Item,int dim>
auto make_FilledArray( auto &&sizes, Item &&value, CtInt<dim> ) {
    return FilledArray<Item,dim>{ FORWARD( sizes ), FORWARD( value ) };
}

END_VFS_NAMESPACE
