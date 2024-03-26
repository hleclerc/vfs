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
    using Next          = Tuple<Tail...>;

    constexpr Tuple     ( auto &&head, auto &&...tail ) : head( FORWARD( head ) ), next( FORWARD( tail )... ) {}
    constexpr Tuple     ( const Tuple &that ) = default;
    constexpr Tuple     ( Tuple &&that ) = default;

    auto      apply     ( auto &&func, auto &&...end_args ) const { return next.apply( FORWARD( func ), FORWARD( end_args )..., head ); }
    auto      apply     ( auto &&func, auto &&...end_args ) { return next.apply( FORWARD( func ), FORWARD( end_args )..., head ); }

    template  <template<typename...> class Filter>
    auto      filtered_apply( auto &&func, auto &&...end_args ) {
        if constexpr ( Filter<Head>::value )
            return next.template filtered_apply<Filter>( FORWARD( func ), FORWARD( end_args )..., head );
        else
            return next.template filtered_apply<Filter>( FORWARD( func ), FORWARD( end_args )... );
    }

    auto  compare      ( const Tuple &t ) const { if ( auto v = VFS_NAMESPACE::compare( head, t.head ) ) return v; return next.compare( t.next ); }

    Head  head;
    Next  next;
};

//
template<>
struct Tuple<> {
    template<template<typename...> class Filter>
    auto filtered_apply( auto &&func, auto &&...end_args ) { return func( FORWARD( end_args )... ); }

    auto compare       ( const Tuple &t ) const { return 0; }

    auto apply         ( auto &&func, auto &&...end_args ) const { return func( FORWARD( end_args )... ); }

};

///
template<class ...Args>
auto tie( Args &...args ) {
    return Tuple<Args& ...>{ args... };
}

// -----------------------------------------------------------------------------------------------------
TA auto *display( auto &ds, const Tuple<A...> &value ) { return value.apply( [&]( const auto &...args ) { return ds.array( { display( ds, args )... } ); } ); }

END_VFS_NAMESPACE
