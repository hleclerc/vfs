#pragma once

#include "../../support/for_each_values_in_md_range.h"
#include "ArrayImpl.h"

BEG_VFS_NAMESPACE

#define DTP template<class Wrapper,class Item,class Capa,class Sizes,class Strides,int alignment_in_bytes,bool need_row_alignment,bool owned>
#define UTP ArrayImpl<Wrapper,Item,Capa,Sizes,Strides,alignment_in_bytes,need_row_alignment,owned>

DTP UTP::ArrayImpl( auto&&...args ) : VfsTdImpl( FORWARD( args )... ) {
}

DTP auto UTP::operator[]( PI index ) const {
    if constexpr ( Capa::size >= 2 )
        return ArrayImpl<Wrapper,Item,typename Capa::Next,typename Sizes::Next,typename Strides::Next,alignment_in_bytes,need_row_alignment,/*owned*/false>{
            FromAttributes(),
            content().strides.tail,
            content().sizes.tail,
            content().capa.tail,
            content().data + content().strides.head * index
        };
    else
        return *reinterpret_cast<const Item *>( content().data + content().strides.head * index );
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
                return tup( nc( wcs, ccs )... ); 
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

DTP void UTP::push_back( auto &&value ) {
    auto new_sizes = sizes.apply( [&]( auto sh, auto ...st ) { return tup( sh + CtInt<1>(), st... ); } );
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
