#pragma once

#include "../../support/for_each_values_in_md_range.h"
#include "ArrayImpl.h"

BEG_VFS_NAMESPACE

#define DTP template<class Wrapper,class Item,class Capa,class Sizes,class Strides,int alignment_in_bytes,bool need_row_alignment,bool owned>
#define UTP ArrayImpl<Wrapper,Item,Capa,Sizes,Strides,alignment_in_bytes,need_row_alignment,owned>

DTP UTP::ArrayImpl( FromReservationSize, auto &&sizes, auto &&capa, auto &&strides ) : strides( FromTupleValues(), FORWARD( strides ) ), sizes( FromTupleValues(), FORWARD( sizes ) ), capa( FromTupleValues(), FORWARD( capa ) ) {
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

DTP UTP::ArrayImpl( FromReservationSize, auto &&sizes, auto &&capa ) : ArrayImpl( FromReservationSize(), FORWARD( sizes ), FORWARD( capa ), tight_strides( capa ) ) {
}

DTP UTP::ArrayImpl( FromReservationSize, auto &&sizes ) : ArrayImpl( FromReservationSize(), sizes, sizes ) {
}

DTP UTP::ArrayImpl( FromSizesAndIterator, auto &&sizes, auto iterator ) : ArrayImpl( FromReservationSize(), sizes ) {
    static_assert( owned == true );

    for_each_indices( [&]( auto ...indices ) {
        new ( data + offset( indices... ) ) Item( *( iterator++ ) );
    } );
}

DTP UTP::ArrayImpl( FromSizesAndValues, auto &&sizes, auto&& ...values ) : ArrayImpl( FromReservationSize(), sizes ) {
    static_assert( owned == true );

    auto set_item = [&]( const auto &value ) { new ( data ) Item( value ); };
    ( set_item( values ), ... );
}

DTP UTP::ArrayImpl( FromAttributes, auto &&strides, auto &&sizes, auto &&capa, char *data ) : strides( FORWARD( strides ) ), sizes( FORWARD( sizes ) ), capa( FORWARD( capa ) ), data( data ) {
}

DTP UTP::ArrayImpl( ArrayImpl &&that ) {
    strides = std::move( that.strides );
    sizes = std::move( that.sizes );
    capa = std::move( that.capa );
    data = std::exchange( that.data, nullptr );
}

DTP UTP::~ArrayImpl() {
    if ( owned && data )
        free( data );
}

DTP auto UTP::offset( auto &&...indices ) const {
    return strides.apply( [&]( auto ...stride_values ) {
        return ( ( stride_values * indices ) + ... );
    } );
}

DTP auto UTP::operator[]( PI index ) const {
    if constexpr ( Capa::size >= 2 )
        return ArrayImpl<Wrapper,Item,typename Capa::Next,typename Sizes::Next,typename Strides::Next,alignment_in_bytes,need_row_alignment,false>{ 
            FromAttributes(),
            strides.tail,
            sizes.tail,
            capa.tail,
            data + strides.head * index
        };
    else
        return *reinterpret_cast<const Item *>( data + strides.head * index );
}

DTP void UTP::set( PI index, auto &&value ) {
    if constexpr ( Capa::size >= 2 )
        TODO;
    else
        *reinterpret_cast<Item *>( data + strides.head * index ) = value;
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
    f( CtInt<owned>() );
}

DTP auto UTP::template_type_name() {
    return "ArrayImpl";
}

// DTP void UTP::for_each_item( const auto &func ) {
//     func( *data );
// }

DTP auto UTP::tight_strides( const auto &capa ) {
    return capa.reversed_tie().prefix_scan_with_index( [&]( auto prod, auto capa_value, auto index ) {
        if constexpr ( index == 0 && Sizes::size > 1 && need_row_alignment && alignment_in_bytes )
            return ceil( prod * capa_value, alignment_in_bytes );
        else
            return prod * capa_value;
    }, CtInt<sizeof(Item)>(), CtInt<0>(), CtInt<1>() );
}

DTP auto UTP::reserve_for( auto &&wanted_capa, auto &&func_on_new_array ) {
    bool out = wanted_capa.apply( [&]( auto... wc ) {
        return capa.apply( [&]( auto... cc ) {
            return ( ( wc > cc ) || ... );
        } );
    } );
    if ( out ) {
        // new attributes
        auto new_capa = wanted_capa.apply( [&]( auto... wcs ) {
            return capa.apply( [&]( auto... ccs ) {
                auto nc = []( auto wc, auto cc ) { return wc >= CtInt<2>() * cc ? wc : CtInt<2>() * cc; };
                return tuple( nc( wcs, ccs )... ); 
            } );
        } );

        auto new_strides = tight_strides( new_capa );

        auto nb = new_capa.apply( [&]( auto ...c ) -> PI {
            const bool nu = ( ( c == 0 ) || ... );
            if ( nu )
                return 0;
            return new_strides.apply( [&]( auto ...s ) {
                return ( ( ( c - 1 ) * s ) + ... ) + sizeof( Item );
            } );
        } );
        auto *new_data = reinterpret_cast<char *>( malloc( nb ) );

        // move content
        new_strides.apply( [&]( auto ...new_stride_values ) {
            strides.apply( [&]( auto ...stride_values ) {
                for_each_indices( [&]( auto ...indices ) {
                    const Item *ov = reinterpret_cast<const Item *>( data + ( ( stride_values * indices ) + ... ) );
                    new ( new_data + ( ( new_stride_values * indices ) + ... ) ) Item( std::move( *ov ) );
                } );
            } );
        } );

        if ( owned && data )
            free( data );

        strides = new_strides;
        // sizes = new_sizes;
        capa = new_capa;
        data = new_data;
    }
    
    func_on_new_array( *this );
    // is size is ct known, we must change the type
    // return ArrayImpl<Wrapper,Item,Capa,decltype(new_sizes),Strides,alignment_in_bytes,need_row_alignment,owned>{ 
    //     FromAttributes(),
    //     strides,
    //     new_sizes,
    //     capa,
    //     data + strides.head * index
    // };
}

DTP void UTP::for_each_indices( auto &&func ) const {
    for_each_values_in_md_range( FORWARD( func ), sizes );
}

DTP void UTP::push_back( auto &&value ) {
    auto new_sizes = sizes.apply( [&]( auto sh, auto ...st ) { return tuple( sh + CtInt<1>(), st... ); } );
    reserve_for( new_sizes, [&]( auto &new_array ) {
        new_array.set( sizes.head, FORWARD( value ) );
        new_array.sizes = new_sizes;
    } );
}

DTP constexpr auto UTP::nb_dims() {
    return Sizes::size();
}

DTP constexpr auto UTP::size() const {
    return sizes.head;
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
