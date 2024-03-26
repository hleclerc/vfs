#pragma once

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


///
template<class ...Args>
auto tie( Args &...args ) {
    return Tuple<Args& ...>{ args... };
}

// impl ---------------------------------------------------------------------------------------------------------
#define DTP template<class Head,class... Tail>
#define UTP Tuple<Head,Tail...>

DTP TTY void UTP::filtered_apply_seq( const auto &func ) const {
    if constexpr ( Y<Head>::value )
        func( head );
    next.template filtered_apply_seq<Y>( func );
}

DTP TTY void UTP::filtered_apply_seq( const auto &func ) {
    if constexpr ( Y<Head>::value )
        func( head );
    next.template filtered_apply_seq<Y>( func );
}

DTP TTY auto UTP::filtered_apply( auto &&func, auto &&...end_args ) const {
    if constexpr ( Y<Head>::value )
        return next.template filtered_apply<Y>( FORWARD( func ), FORWARD( end_args )..., head );
    else
        return next.template filtered_apply<Y>( FORWARD( func ), FORWARD( end_args )... );
}

DTP TTY auto UTP::filtered_apply( auto &&func, auto &&...end_args ) {
    if constexpr ( Y<Head>::value )
        return next.template filtered_apply<Y>( FORWARD( func ), FORWARD( end_args )..., head );
    else
        return next.template filtered_apply<Y>( FORWARD( func ), FORWARD( end_args )... );
}

#undef DTP
#undef UTP

// -----------------------------------------------------------------------------------------------------
TA auto *display( auto &ds, const Tuple<A...> &value ) { return value.apply( [&]( const auto &...args ) { return ds.array( { display( ds, args )... } ); } ); }

END_VFS_NAMESPACE
