#pragma once

#include "StorageTypeFor.h"
#include "TypeConfig.h"
#include "compare.h"

BEG_VFS_NAMESPACE

/// a simple replacement for tuple, providing head and next attributes
///  + values in the natural ordering
template<class... Items>
struct Tuple;

//
template<class Head,class... Tail>
struct Tuple<Head,Tail...> {
    TTY using                    Filtered          = std::conditional_t<Y<Head>::value,typename Tuple<Tail...>::template Filtered<Y>::template Prepend<Head>,typename Tuple<Tail...>::template Filtered<Y>>;
    TT using                     Prepend           = Tuple<T,Head,Tail...>;
    using                        Next              = Tuple<Tail...>;
    static constexpr std::size_t size              = 1 * Next::size;
    TTY struct                   Map               { using value = Tuple<typename Y<Head>::value,typename Y<Tail>::value...>; };

    constexpr                    Tuple             ( FromTupleValues, auto &&tuple ) : head( tuple.head ), next( FromTupleValues(), tuple.next ) {}
    constexpr                    Tuple             ( auto &&head, auto &&...tail ) requires ( std::is_same_v<Tuple,DECAYED_TYPE_OF(head)> == false ) : head( FORWARD( head ) ), next( FORWARD( tail )... ) {}
    constexpr                    Tuple             ( const Tuple &that ) = default;
    constexpr                    Tuple             ( Tuple &&that ) = default;
    constexpr                    Tuple             () = default;

    std::ptrdiff_t               compare           ( const Tuple &t ) const { if ( auto v = VFS_NAMESPACE::compare( head, t.head ) ) return v; return next.compare( t.next ); }

    TTY void                     filtered_apply_seq( const auto &func ) const;
    TTY void                     filtered_apply_seq( const auto &func );
    TTY auto                     filtered_apply    ( auto &&func, auto &&...end_args ) const;
    TTY auto                     filtered_apply    ( auto &&func, auto &&...end_args );
    auto                         apply             ( auto &&func, auto &&...end_args ) const { return next.apply( FORWARD( func ), FORWARD( end_args )..., head ); }
    auto                         apply             ( auto &&func, auto &&...end_args ) { return next.apply( FORWARD( func ), FORWARD( end_args )..., head ); }

    // auto                      append            ( auto &&...args ) { return Tuple<Head,Tail...,>; }

    Head                         head;
    Next                         next;
};

//
template<>
struct Tuple<> {
    TTY using                    Filtered          = Tuple;
    TT using                     Prepend           = Tuple<T>;
    static constexpr std::size_t size              = 0;
    TTY struct                   Map               { using value = Tuple<>; };

    constexpr                    Tuple             ( FromTupleValues, auto &&tuple ) {}
    constexpr                    Tuple             () {}

    std::ptrdiff_t               compare           ( const Tuple &t ) const { return 0; }

    TTY void                     filtered_apply_seq( const auto &func ) const {}
    TTY auto                     filtered_apply    ( auto &&func, auto &&...end_args ) const { return func( FORWARD( end_args )... ); }
    auto                         apply             ( auto &&func, auto &&...end_args ) const { return func( FORWARD( end_args )... ); }
};

// ctor functions ------------------------------------------------------------------------------------------------------------

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

// tuple_cat ------------------------------------------------------------------------------------------------------------------

///
template<class... A,class... B>
auto tuple_cat( Tuple<A...> &&a, Tuple<B...> &&b ) {
    return a.apply( [&]( auto &...va ) {
        return b.apply( [&]( auto &...vb ) {
            return Tuple<A...,B...>( std::move( va )..., std::move( vb )... );
        } );
    } );
}

///
template<class... A,class... B>
auto tuple_cat( const Tuple<A...> &a, const Tuple<B...> &b ) {
    return a.apply( [&]( auto &...va ) {
        return b.apply( [&]( auto &...vb ) {
            return Tuple<A...,B...>( va..., vb... );
        } );
    } );
}

///
auto tuple_cat( auto &&a, auto &&b, auto &&c ) { return tuple_cat( tuple_cat( a, b ), c ); }
auto tuple_cat( auto &&a ) { return a; }

// ext functions ---------------------------------------------------------------------------------------------------------------
TA auto *display( auto &ds, const Tuple<A...> &value ) { return value.apply( [&]( const auto &...args ) { return ds.array( { display( ds, args )... } ); } ); }

END_VFS_NAMESPACE
