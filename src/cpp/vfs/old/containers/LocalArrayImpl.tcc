#pragma once

#include "ArrayImpl.h"

/// homogeneous dynamic vector ---------------------------------------------------------------------
#define DTP template<class Item,int alig>
#define UTP GenericArray<SL<Item>,NL<-1>,NL<alig>,Memory_Cpu>

DTP TTA UTP::GenericArray( FromSizeAndInitItemFunction, Memory_Cpu m, PI size, const T &func, const A &...add_args ) : GenericArray( FromSizeAndUninitializedItemValues(), m, size ) {
    for( PI i = 0; i < size; ++i )
        func( data_ + i, add_args... );
}

DTP TA UTP::GenericArray( FromSizeAndItemValue, Memory_Cpu m, PI size, const A &...value ) : GenericArray( FromReservationSize(), m, size ) {
    for( PI i = 0; i < size; ++i )
        push_back_in_reserved( value... );
}

DTP UTP::GenericArray( FromSizeAndExternalItemValues, Memory_Cpu, PI size, const void *data_ptr ) {
    data_ = const_cast<Item *>( reinterpret_cast<const Item *>( data_ptr ) );
    size_ = size;
    capa_ = 0;
}

DTP UTP::GenericArray( FromSizeAndUninitializedItemValues, Memory_Cpu m, PI size ) : GenericArray( FromReservationSize(), m, size ) {
    size_ = size;
}

DTP UTP::GenericArray( FromReservationSize, Memory_Cpu, PI capa ) {
    init_with_room_for( capa );
}

DTP UTP::GenericArray( FromIterator, Memory_Cpu, auto iter_data, IteratorLike auto iter_size ) {
    init_with_room_for( *iter_size );
    for( PI i = 0; i < *iter_size; ++i )
        push_back_in_reserved( *( iter_data++ ) );
}

DTP UTP::GenericArray( FromIterator, Memory_Cpu, auto iter_data, auto iter_size ) : GenericArray( FromIterator(), Memory_Cpu(), iter_data, &iter_size ) {
}

DTP UTP::~GenericArray() {
    free();
}

DTP Item &UTP::value_at_off_and_inds( S<Item>, PI off, PI ind ) {
    return data_[ off ];
}

DTP TT void UTP::for_each_off_and_inds( T &&func ) const {
    for( PI i = 0; i < size_; ++i )
        func( i, i );
}

DTP void UTP::raw_size_assignation( PI size ) {
    size_ = size;
}

DTP void UTP::init_with_room_for( PI capa ) {
    data_ = allocate( capa );
    capa_ = capa;
    size_ = 0;
}

DTP void UTP::clear() {
    if ( ! std::is_trivially_destructible_v<Item> )
        while ( size_-- )
            data_[ size_ ].~Item();
}

DTP void UTP::free() {
    clear();
    if ( capa_ ) {
        std::free( data_ );
        capa_ = 0;
    }
}

DTP Item *UTP::allocate( PI nb_items ) {
    // static T*        allocate             ( PI nb_items ) { return reinterpret_cast<T *>( aligned_alloc( alig_in_bytes, sizeof( T ) * nb_T ) ); } P( new_rese, (PI)data_, sizeof( T ), type_name<T>() );
    // static T*        allocate             ( PI nb_items ) { return reinterpret_cast<T *>( std::aligned_alloc( alig_in_bytes, sizeof( T ) * nb_T ) ); }
    return reinterpret_cast<Item *>( std::malloc( sizeof( Item ) * nb_items ) );
}

DTP DisplayerItem *UTP::display( Displayer &ds ) const {
    return ds.array( [&]( const auto &f ) {
        for( PI i = 0; i < size_; ++i )
            f( ::display( ds, data_[ i ] ) );
    } );
}

#undef DTP
#undef UTP

/// static vector ---------------------------------------------------------------------
#define DTP template<class Item,int ct_size,int alig>
#define UTP GenericArray<SL<Item>,NL<ct_size>,NL<alig>,Memory_Cpu>

DTP TTA UTP::GenericArray( FromSizeAndInitItemFunction, Memory_Cpu m, PI, const T &func, const A &...add_args ) {
    for_each_item( [&]( auto &item ) {
        func( &item, add_args... );
    } );
}

DTP TA UTP::GenericArray( FromSizeAndItemValue, Memory_Cpu, auto, A&&... ctor_args ) {
    for_each_item( [&]( auto &item ) {
        new ( &item ) Item( ctor_args... );
    } );
}

DTP UTP::GenericArray( FromIterator, Memory_Cpu, auto iter_data, IteratorLike auto iter_size ) {
    for( PI i = 0; i < *iter_size; ++i )
        new ( data_ + i * sizeof( Item ) ) Item( *( iter_data++ ) );
}

DTP UTP::GenericArray( FromIterator, Memory_Cpu, auto iter_data, auto iter_size ) : GenericArray( FromIterator(), Memory_Cpu(), iter_data, &iter_size ) {
}

DTP UTP::GenericArray() {
    for_each_item( [&]( auto &item ) {
        new ( &item ) Item;
    } );
}

// DTP TA UTP::GenericArray( FromItemValues, A&&... values ) {
//     char *ptr = data;
//     ( new ( std::exchange( ptr, ptr + sizeof( Items ) ) ) Items( std::forward<A>( values ) ), ... );
// }

DTP UTP::~GenericArray() {
    for_each_item( [&]( auto &item ) {
        using T = std::decay_t<decltype( item )>;
        item.~T();
    } );
}

DTP TT void UTP::for_each_item( T &&func ) const {
    for( PI i = 0; i < ct_size; ++i )
        func( *reinterpret_cast<const Item *>( data_ + i * sizeof( Item ) ) );
}

DTP TT void UTP::for_each_item( T &&func ) {
    for( PI i = 0; i < ct_size; ++i )
        func( *reinterpret_cast<Item *>( data_ + i * sizeof( Item ) ) );
}

DTP DisplayerItem *UTP::display( Displayer &ds ) const {
    return ds.array( [&]( const auto &f ) {
        for_each_item( [&]( const auto &item ) {
            f( ::display( ds, item ) );
        } );
    } );
}

DTP const Item &UTP::select( PI index ) const {
    return *reinterpret_cast<const Item *>( data_ + index * sizeof( Item ) );
}

DTP Item &UTP::select( PI index ) {
    return *reinterpret_cast<Item *>( data_ + index * sizeof( Item ) );
}

#undef DTP
#undef UTP
// /// static vector ---------------------------------------------------------------------
// #define DTP template<class... Items,int ct_size,int alig>
// #define UTP GenericArray<SL<Items...>,NL<ct_size>,NL<alig>,Memory_Cpu>

// DTP TA UTP::GenericArray( FromItemValues, A&&... values ) {
//     char *ptr = data;
//     ( new ( std::exchange( ptr, ptr + sizeof( Items ) ) ) Items( std::forward<A>( values ) ), ... );
// }

// DTP UTP::~GenericArray() {
//     for_each_item( [&]( auto &item ) {
//         using T = std::decay_t<decltype( item )>;
//         item.~T();
//     } );
// }

// DTP TT void UTP::for_each_item( T &&func ) const {
//     const char *ptr = data;
//     ( func( *reinterpret_cast<const Items *>( std::exchange( ptr, ptr + sizeof( Items ) ) ) ), ... );
// }

// DTP TT void UTP::for_each_item( T &&func ) {
//     char *ptr = data;
//     ( func( *reinterpret_cast<Items *>( std::exchange( ptr, ptr + sizeof( Items ) ) ) ), ... );
// }

// DTP DisplayerItem *UTP::display( Displayer &ds ) const {
//     return ds.array( [&]( const auto &f ) {
//         for_each_item( [&]( const auto &item ) {
//             f( ::display( ds, item ) );
//         } );
//     } );
// }

// #undef DTP
// #undef UTP

