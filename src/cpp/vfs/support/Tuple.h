#pragma once

#include "StorageTypeFor.h"
#include "common_macros.h"
#include "compare.h"

BEG_VFS_NAMESPACE

/// a simple replacement for tuple, providing head and next attributes
///  + values in the natural ordering
template<class... Items>
struct Tuple;

//
template<class Head,class... Tail>
struct Tuple<Head,Tail...> {
    TTY using      Filtered          = std::conditional_t<Y<Head>::value,typename Tuple<Tail...>::template Filtered<Y>::template Prepend<Head>,typename Tuple<Tail...>::template Filtered<Y>>;
    TT using       Prepend           = Tuple<T,Head,Tail...>;
    using          Next              = Tuple<Tail...>;

    constexpr      Tuple             ( auto &&head, auto &&...tail ) requires ( std::is_same_v<Tuple,DECAYED_TYPE_OF(head)> == false ) : head( FORWARD( head ) ), next( FORWARD( tail )... ) {}
    constexpr      Tuple             ( const Tuple &that ) = default;
    constexpr      Tuple             ( Tuple &&that ) = default;
    constexpr      Tuple             () = default;

    std::ptrdiff_t compare           ( const Tuple &t ) const { if ( auto v = VFS_NAMESPACE::compare( head, t.head ) ) return v; return next.compare( t.next ); }

    TTY void       filtered_apply_seq( const auto &func ) const;
    TTY void       filtered_apply_seq( const auto &func );
    TTY auto       filtered_apply    ( auto &&func, auto &&...end_args ) const;
    TTY auto       filtered_apply    ( auto &&func, auto &&...end_args );
    auto           apply             ( auto &&func, auto &&...end_args ) const { return next.apply( FORWARD( func ), FORWARD( end_args )..., head ); }
    auto           apply             ( auto &&func, auto &&...end_args ) { return next.apply( FORWARD( func ), FORWARD( end_args )..., head ); }

    // auto           append            ( auto &&...args ) { return Tuple<Head,Tail...,>; }

    Head           head;
    Next           next;
};

//
template<>
struct Tuple<> {
    TTY using      Filtered          = Tuple;
    TT using       Prepend           = Tuple<T>;

    std::ptrdiff_t compare           ( const Tuple &t ) const { return 0; }

    TTY void       filtered_apply_seq( const auto &func ) const {}
    TTY auto       filtered_apply    ( auto &&func, auto &&...end_args ) const { return func( FORWARD( end_args )... ); }
    auto           apply             ( auto &&func, auto &&...end_args ) const { return func( FORWARD( end_args )... ); }
};

// helper functions ------------------------------------------------------------------------------------------------------------

///
template<class ...Args>
auto tie( Args &...args ) {
    return Tuple<Args& ...>{ args... };
}

///
template<class ...Args>
auto tuple( Args &&...args ) {
    return Tuple<typename StorageTypeFor<Args>::value...>{ FORWARD( args )... };
}

///
template<class... A,class... B>
auto tuple_cat( Tuple<A...> &&a, Tuple<B...> &&b ) {
    return a.apply( [&]( auto &...va ) {
        return b.apply( [&]( auto &...vb ) {
            return Tuple<A...,B...>( std::move( va )..., std::move( vb )... );
        } );
    } );
}

// ext functions ---------------------------------------------------------------------------------------------------------------
TA auto *display( auto &ds, const Tuple<A...> &value ) { return value.apply( [&]( const auto &...args ) { return ds.array( { display( ds, args )... } ); } ); }

END_VFS_NAMESPACE
