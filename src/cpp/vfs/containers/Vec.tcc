#pragma once

#include "select_with_n_indices.h"
#include "Seq.h"

BEG_VFS_NAMESPACE

/// static vector ---------------------------------------------------------------------
#define DTP template<class Item,int static_size,int local_size,int alignment,bool allow_heap>
#define UTP Seq<Item,static_size,local_size,alignment,allow_heap>

DTP UTP::Seq( FromOperationOnItemsOf, auto op_name, auto nb_indices_to_take, auto &&...operands ) {
    for( PI index = 0; index < PI( size() ); ++index ) {
        nb_indices_to_take.apply( [&]( auto... nb_indices_to_take ) {
            new ( data( index ) ) Item( call_by_name( op_name, select_with_n_indices( operands, nb_indices_to_take, index )... ) );
        } );
    }
}

DTP UTP::Seq( FromItemValues, auto &&...values ) {
    PI index = 0;

    static_assert( sizeof...( values ) <= static_size );
    ( new ( data( index++ ) ) Item( FORWARD( values ) ), ... );

    while( index < static_size )
        new ( data( index++ ) ) Item;
}

DTP UTP::Seq( FromItemValue, auto &&...ctor_args ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( ctor_args... );
}

DTP UTP::Seq( FromIterator, auto iter ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( *( iter++ ) );
}

DTP TT UTP::Seq( const std::initializer_list<T> &lst ) {
    auto iter = lst.begin();
    for( PI index = 0; index < min( lst.size(), size() ); ++index )
        new ( data( index ) ) Item( *(iter++) );

    for( PI index = lst.size(); index < size(); ++index )
        new ( data( index ) ) Item;
}

DTP UTP::Seq( const HasSizeAndAccess auto &l ) {
    using namespace std;
    if constexpr( requires { l[ 0 ]; } ) {
        for( PI index = 0; index < min( size(), l.size() ); ++index )
            new ( data( index ) ) Item( l[ index ] );
    } else {
        PI index = 0;
        for( const auto &v : l ) {
            if ( index >= size() )
                break;
            new ( data( index++ ) ) Item( v );
        }
    }

    for( PI index = l.size(); index < size(); ++index )
        new ( data( index ) ) Item;
}

DTP UTP::Seq( const Seq &that ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( that[ index ] );
}

DTP UTP::Seq( Seq &&that ) {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item( std::move( that[ index ] ) );
}

DTP UTP::Seq() {
    for( PI index = 0; index < size(); ++index )
        new ( data( index ) ) Item;
}

DTP UTP::~Seq() {
    for( PI i = static_size; i--; )
        data( i )->~Item();
}

DTP UTP &UTP::operator=( const Seq &that ) {
    for( PI i = 0; i < size(); ++i )
        operator[]( i ) = that[ i ];
    return *this;
}

DTP UTP &UTP::operator=( Seq &&that ) {
    for( PI i = 0; i < size(); ++i )
        operator[]( i ) = std::move( that[ i ] );
    return *this;
}

DTP const Item &UTP::operator[]( PI index ) const {
    return data()[ index ];
}

DTP Item &UTP::operator[]( PI index ) {
    return data()[ index ];
}

DTP const Item &UTP::operator()( PI index ) const {
    return data()[ index ];
}

DTP Item &UTP::operator()( PI index ) {
    return data()[ index ];
}

DTP const Item *UTP::data( PI index ) const {
    return data() + index;
}

DTP Item *UTP::data( PI index ) {
    return data() + index;
}

DTP const Item *UTP::data() const {
    return reinterpret_cast<const Item *>( data_ );
}

DTP Item *UTP::data() {
    return reinterpret_cast<Item *>( data_ );
}

#undef DTP
#undef UTP

/// dynamic vector ---------------------------------------------------------------------
#define DTP template<class Item,int alignment>
#define UTP Seq<Item,-1,0,alignment,true>

DTP UTP::Seq( FromSizeAndInitFunctionOnIndex, PI size, auto &&func ) : Seq( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        func( data_ + index, index );
}

DTP UTP::Seq( FromSizeAndItemValue, PI size, auto &&...ctor_args ) : Seq( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        new ( data_ + index ) Item( FORWARD( ctor_args )... );
}

DTP UTP::Seq( FromSize, PI size ) : Seq( FromReservationSize(), size, size ) {
    for( PI index = 0; index < size; ++index )
        new ( data_ + index ) Item;
}

DTP UTP::Seq( FromOperationOnItemsOf, auto &&a, auto &&operation ) : Seq( FromReservationSize(), a.size(), a.size() ) {
    constexpr bool move = std::is_rvalue_reference_v<decltype(a)>;
    for( PI index = 0; index < a.size(); ++index )
        new ( data_ + index ) Item( operation( move ? std::move( a[ index ] ) : a[ index ] ) );
}

DTP UTP::Seq( FromReservationSize, PI capa, PI raw_size ) {
    data_ = allocate( capa );
    size_ = raw_size;
    capa_ = capa;
}

DTP UTP::Seq( FromItemValues, auto &&...values ) : Seq( FromReservationSize(), sizeof...( values ), sizeof...( values ) ) {
    PI index = 0;
    ( new ( data_ + index++ ) Item( FORWARD( values ) ), ... );
}

DTP UTP::Seq( const std::initializer_list<Item> &l ) : Seq( FromReservationSize(), l.size(), l.size() ) {
    PI index = 0;
    for( const Item &v : l )
        new ( data_ + index++ ) Item( v );
}

DTP UTP::Seq( const HasSizeAndAccess auto &l ) : Seq( FromReservationSize(), l.size(), l.size() ) {
    if constexpr( requires { l[ 0 ]; } ) {
        for( PI index = 0; index < l.size(); ++index )
            new ( data_ + index ) Item( l[ index ] );
    } else {
        PI index = 0;
        for( const auto &v : l )
            new ( data_ + index++ ) Item( v );
    }
}

DTP UTP::Seq( const Seq &that ) : Seq( FromReservationSize(), that.size(), that.size() ) {
    for( PI index = 0; index < that.size(); ++index )
        new ( data_ + index ) Item( that[ index ] );
}

DTP UTP::Seq( Seq &&that ) {
    data_ = std::exchange( that.data_, nullptr );
    size_ = std::exchange( that.size_, 0 );
    capa_ = std::exchange( that.capa_, 0 );
}

DTP UTP::Seq() : Seq( FromReservationSize(), 0, 0 ) {
}

DTP UTP::~Seq () {
    if ( capa_ ) {
        for( PI i = size(); i--; )
            data( i )->~Item();
        std::free( data_ );
    }
}

DTP UTP &UTP::operator=( const Seq &that ) {
    // need more room ?
    if ( capa_ < that.size() ) {
        if ( capa_ ) {
            for( PI i = size(); i--; )
                data( i )->~Item();
            std::free( data_ );
        } else
            capa_ = 1;

        while ( capa_ < that.size() )
            capa_ *= 2;

        data_ = allocate( capa_ );
        size_ = that.size_;
        for( PI i = 0; i < that.size_; ++i )
            new ( data_ + i ) Item( that[ i ] );
        return *this;
    }

    // else, copy in place
    for( PI i = 0; i < std::min( size_, that.size_ ); ++i )
        *data( i ) = that[ i ];
    for( ; size_ < that.size_; ++size_ )
        new ( data_ + size_ ) Item( that[ size_ ] );
    for( ; size_ > that.size_; )
        data( --size_ )->~Item();

    return *this;
}

DTP UTP &UTP::operator=( Seq &&that ) {
    if ( capa_ ) {
        for( PI i = size(); i--; )
            data( i )->~Item();
        std::free( data_ );
    }
    data_ = std::exchange( that.data_, nullptr );
    size_ = std::exchange( that.size_, 0 );
    capa_ = std::exchange( that.capa_, 0 );
    return *this;
}

DTP const Item &UTP::operator[]( PI index ) const {
    return data_[ index ];
}

DTP Item &UTP::operator[]( PI index ) {
    return data_[ index ];
}

DTP const Item *UTP::data( PI index ) const {
    return data_ + index;
}

DTP Item *UTP::data( PI index ) {
    return data_ + index;
}

DTP const Item *UTP::data() const {
    return data_;
}

DTP Item *UTP::data() {
    return data_;
}

DTP PI UTP::size() const {
    return size_;
}

DTP Item UTP::pop_back_val() {
    Item res = std::move( back() );
    data_[ size_-- ].~Item();
    return res;
}

DTP void UTP::push_back_br( auto&&...args ) {
    reserve( size_ + 1 );
    new ( data_ + size_++ ) Item{ FORWARD( args )... };
}

DTP void UTP::push_back( auto&&...args ) {
    reserve( size_ + 1 );
    new ( data_ + size_++ ) Item( FORWARD( args )... );
}

DTP void UTP::resize( PI size, auto&&...ctor_args ) {
    reserve( size );

    while( size_ > size )
        data_[ --size_ ].~Item();

    while( size_ < size )
        new ( data_ + size_++ ) Item( FORWARD( ctor_args )... );
}

DTP void UTP::append( auto &&that ) {
    bool move = std::is_rvalue_reference_v<decltype(that)>;
    // if constexpr( ... )
    reserve( size_ + that.size() );
    if ( move )
        for( PI i = 0; i < that.size(); ++i )
            new ( data_ + size_++ ) Item( std::move( that[ i ] ) );
    else
        for( PI i = 0; i < that.size(); ++i )
            new ( data_ + size_++ ) Item( that[ i ] );
}

DTP void UTP::clear() {
    while( size_ )
        data_[ --size_ ].~Item();
}

DTP void UTP::reserve( PI tgt_capa ) {
    if ( capa_ >= tgt_capa )
        return;

    PI new_capa = capa_ ? capa_ : 1;
    while ( new_capa < tgt_capa )
        new_capa *= 2;

    Item *new_data = allocate( new_capa );
    for( PI i = 0; i < size_; ++i )
        new ( new_data + i ) Item( std::move( data_[ i ] ) );
    for( PI i = size_; i--; )
        data_[ i ].~Item();

    if ( capa_ )
        std::free( data_ );
    capa_ = new_capa;
    data_ = new_data;
}

DTP void UTP::copy_data_to( void *data ) const {
    for( PI i = 0; i < size_; ++i )
        new ( reinterpret_cast<Item *>( data ) + i ) Item( data_[ i ] );
}

DTP Item *UTP::allocate( PI nb_items ) {
    constexpr PI al = std::max( PI( alignment ), alignof( Item ) );
    //return nb_items ? reinterpret_cast<Item *>( std::malloc( sizeof( Item ) * nb_items ) ) : nullptr;
    return nb_items ? reinterpret_cast<Item *>( std::aligned_alloc( al, sizeof( Item ) * nb_items ) ) : nullptr;
}

DTP UTP UTP::range( Item end ) {
    return { FromSizeAndInitFunctionOnIndex{}, end, []( Item *item, PI index ) { new ( item ) Item( index ); } };
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
