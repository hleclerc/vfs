#pragma once

#include "../containers/Span.h"

BEG_VFS_NAMESPACE

/// func( size(), sample_item_ptr ). Beware: sample_item_ptr may be a typed or untyped nullptr.
constexpr auto with_sizes_and_sample_item_ptr( const auto &that, auto &&func ) {
    if constexpr ( requires { that.sizes(); } ) {
        return func( that.sizes(), that.sample_item_ptr() );
    } else {
        auto size = that.size();
        return func( Span<decltype( size ),1>{ &size } );
    }
}


// constexpr auto with_sample_item_and_sizes( const auto &that, CtInt<0>, auto &&func, const auto &...sizes_so_far ) {
//     if constexpr ( requires { that.sizes(); } )
//         return func( that.sizes() );
//     else {
//     }
// }

// Ti constexpr auto with_sample_item_and_sizes( const auto &that, CtInt<i>, auto &&func, const auto &...sizes_so_far ) {
//     if ( auto size = that.size() )
//         return with_sizes_and_sample_item( that[ 0 ], CtInt<i-1>(), FORWARD( func ), sizes_so_far..., size );
//     else
//         return with_sizes_and_sample_item( that[ 0 ], CtInt<i-1>(), FORWARD( func ), sizes_so_far..., size );
// }

END_VFS_NAMESPACE
