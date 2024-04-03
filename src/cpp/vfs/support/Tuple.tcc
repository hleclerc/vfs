#pragma once

#include "Tuple.h"

BEG_VFS_NAMESPACE

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

END_VFS_NAMESPACE
