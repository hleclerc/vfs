#pragma once

#include "../../support/for_each_values_in_md_range.h"
#include "ArrayImplData.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item, class Strides, class Shape, class Capa, int alignment_in_bytes, bool need_row_alignment, bool owned>
#define UTP ArrayImplData<Item, Strides, Shape, Capa, alignment_in_bytes, need_row_alignment, owned>

DTP UTP::ArrayImplData( FromReservationSize, auto &&sizes, auto &&capa, auto &&strides ) : strides( FromTupleValues(), FORWARD( strides ) ), shape( FromTupleValues(), FORWARD( shape ) ), capa( FromTupleValues(), FORWARD( capa ) ) {
    auto nb = capa.apply( [&]( auto ...c ) -> PI {
        const bool nu = ( ( c == 0 ) || ... );
        if ( nu )
            return 0;
        return strides.apply( [&]( auto ...s ) {
            return ( ( ( c - 1 ) * s ) + ... ) + sizeof( Item );
        } );
    } );
    data = reinterpret_cast<char *>( malloc( nb ) );
}

DTP UTP::ArrayImplData( FromReservationSize, auto &&sizes, auto &&capa ) : ArrayImplData( FromReservationSize(), FORWARD( sizes ), FORWARD( capa ), tight_strides( capa ) ) {
}

DTP UTP::ArrayImplData( FromReservationSize, auto &&sizes ) : ArrayImplData( FromReservationSize(), sizes, sizes ) {
}

DTP UTP::ArrayImplData( FromShapeAndItemValue, auto &&sizes, auto &&item ) : ArrayImplData( FromReservationSize(), sizes ) {
    static_assert( owned == true );

    for_each_indices( [&]( auto ...indices ) {
        new ( data + offset( indices... ) ) Item( item );
    } );
}

DTP UTP::ArrayImplData( FromShapeAndIterator, auto &&sizes, auto iterator ) : ArrayImplData( FromReservationSize(), sizes ) {
    static_assert( owned == true );

    for_each_indices( [&]( auto ...indices ) {
        new ( data + offset( indices... ) ) Item( *( iterator++ ) );
    } );
}

DTP UTP::ArrayImplData( FromShapeAndValues, auto &&sizes, auto&& ...values ) : ArrayImplData( FromReservationSize(), sizes ) {
    static_assert( owned == true );

    auto set_item = [&]( const auto &value ) { new ( data ) Item( value ); };
    ( set_item( values ), ... );
}

DTP UTP::ArrayImplData( FromAttributes, auto &&strides, auto &&sizes, auto &&capa, char *data ) : strides( FORWARD( strides ) ), shape( FORWARD( shape ) ), capa( FORWARD( capa ) ), data( data ) {
}

DTP UTP::ArrayImplData( ArrayImplData &&that ) {
    strides = std::move( that.strides );
    shape = std::move( that.shape );
    capa = std::move( that.capa );
    data = std::exchange( that.data, nullptr );
}

DTP UTP::~ArrayImplData() {
    if ( owned && data )
        free( data );
}

DTP void UTP::for_each_indices( auto &&func ) const {
    for_each_values_in_md_range( FORWARD( func ), shape );
}

DTP auto UTP::offset( auto &&...indices ) const {
    return strides.apply( [&]( auto ...stride_values ) {
        return ( ( stride_values * indices ) + ... );
    } );
}

DTP auto UTP::tight_strides( const auto &capa ) {
    return capa.reversed_tie().prefix_scan_with_index( [&]( auto prod, auto capa_value, auto index ) {
        if constexpr ( index == 0 && Shape::size > 1 && need_row_alignment && alignment_in_bytes )
            return ceil( prod * capa_value, alignment_in_bytes );
        else
            return prod * capa_value;
    }, CtInt<sizeof(Item)>(), CtInt<0>(), CtInt<1>() );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
