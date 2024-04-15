#pragma once

#include "ArrayImpl.h"

BEG_VFS_NAMESPACE

#define DTP template<class Wrapper,class Item,class Capa,class Sizes,class Strides,int alignment_in_bytes,bool need_row_alignment>
#define UTP ArrayImpl<Wrapper,Item,Capa,Sizes,Strides,alignment_in_bytes,need_row_alignment>

DTP UTP::ArrayImpl( FromReservationSize, auto &&strides, auto &&sizes, auto &&capa ) : strides( FromTupleValues(), FORWARD( strides ) ), sizes( FromTupleValues(), FORWARD( sizes ) ), capa( FromTupleValues(), FORWARD( capa ) ) {
    data = reinterpret_cast<Item *>( malloc( strides.head * capa.head ) );
    P( sizeof( strides ) );
    P( sizeof( sizes ) );
    P( sizeof( capa ) );
    P( strides );
}

DTP UTP::ArrayImpl( FromSizesAndValues, auto &&sizes, auto&& ...values ) : ArrayImpl( FromReservationSize(), tight_strides( sizes ), sizes, sizes ) {
    auto set_item = [&]( const auto &value ) { new ( data ) Item( value ); };
    ( set_item( values ), ... );
}

DTP UTP::ArrayImpl( ArrayImpl &&that ) {
    strides = std::move( that.strides );
    sizes = std::move( that.sizes );
    capa = std::move( that.capa );
    data = std::exchange( that.data, nullptr );
}

DTP UTP::~ArrayImpl() {
    if ( data )
        free( data );
}

DTP void UTP::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/impl/Array/ArrayImpl.h" );
}

DTP void UTP::for_each_template_arg( auto &&f ) {
    f( CtType<Wrapper>() );
    f( CtType<Item>() );
    f( CtType<Capa>() );
    f( CtType<Sizes>() );
    f( CtType<Strides>() );
    f( CtInt<alignment_in_bytes>() );
    f( CtInt<need_row_alignment>() );
}

DTP auto UTP::template_type_name() {
    return "ArrayImpl";
}

DTP void UTP::for_each_item( const auto &func ) {
    func( *data );
}

DTP auto UTP::tight_strides( const Capa &capa ) {
    P( capa.tail );
    P( capa );
    auto res = capa.prefix_scan_with_index( [&]( auto prod, auto capa_value, auto index ) {
        PT( capa_value * prod );
        PT( capa_value );
        PT( prod );
        if constexpr ( index == 0 && Sizes::size > 1 && need_row_alignment && alignment_in_bytes )
            return ceil( prod * capa_value, alignment_in_bytes );
        else
            return prod * capa_value;
    }, CtInt<sizeof(Item)>(), CtInt<0>(), CtInt<1>() );
    P( res );
    TODO;
    return res;
}

DTP constexpr auto UTP::nb_dims() {
    return Sizes::size();
}

DTP constexpr auto UTP::size() {
    return sizes.head;
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
