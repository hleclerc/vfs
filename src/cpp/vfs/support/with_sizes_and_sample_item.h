#pragma once

#include "../containers/Span.h"

BEG_VFS_NAMESPACE

/**
  Pb: on ne peut pas obtenir les tailles en se basant uniquement sur les échantillons


*/
/// call func( sizes, &a_sample_item_or_nullptr_if_not_possible )
constexpr auto with_sample_item_and_sizes( const auto &that, CtInt<0>, auto &&func, const auto &...sizes_so_far ) {
    if constexpr ( requires { that.sizes(); } )
        return func( that.sizes() );
    else {
        PI size = that.size();
        return func( Span<PI,1>{ &size } );
    }
}

Ti constexpr auto with_sample_item_and_sizes( const auto &that, CtInt<i>, auto &&func, const auto &...sizes_so_far ) {
    if ( auto size = that.size() )
        return with_sizes_and_sample_item( that[ 0 ], CtInt<i-1>(), FORWARD( func ), sizes_so_far..., size );
    else
        return with_sizes_and_sample_item( that[ 0 ], CtInt<i-1>(), FORWARD( func ), sizes_so_far..., size );
}

END_VFS_NAMESPACE