#pragma once

#include "../../support/common_ctor_selectors.h"

BEG_VFS_NAMESPACE

///
template<class Item, class Strides, class Shape, class Capa, int alignment_in_bytes, bool need_row_alignment, bool owned>
struct ArrayImplData {
    /**/        ArrayImplData   ( FromReservationSize, auto &&sizes, auto &&capa, auto &&strides );
    /**/        ArrayImplData   ( FromReservationSize, auto &&sizes, auto &&capa );
    /**/        ArrayImplData   ( FromReservationSize, auto &&sizes );
    /**/        ArrayImplData   ( FromShapeAndItemValue, auto &&sizes, auto &&item );
    /**/        ArrayImplData   ( FromShapeAndIterator, auto &&sizes, auto iterator );
    /**/        ArrayImplData   ( FromShapeAndValues, auto &&sizes, auto &&...values );
    /**/        ArrayImplData   ( FromAttributes, auto &&strides, auto &&sizes, auto &&capa, char *data );

    /**/        ArrayImplData   ( const ArrayImplData &that ) = delete;
    /**/        ArrayImplData   ( ArrayImplData &&that );

    /**/       ~ArrayImplData   ();

    void        operator=       ( const ArrayImplData &that ) = delete;
    void        operator=       ( ArrayImplData &&that ) = delete;

    void        for_each_indices( auto &&func ) const;
    auto        offset          ( auto &&...indices ) const; ///< in bytes

    static auto tight_strides   ( const auto &capa );

    NUA Strides strides;
    NUA Shape   shape;
    NUA Capa    capa;
    NUA char*   data;
};

END_VFS_NAMESPACE

#include "ArrayImplData.tcc"
