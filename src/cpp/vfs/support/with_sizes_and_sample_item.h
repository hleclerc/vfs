#pragma once

#include "../containers/Span.h"

BEG_VFS_NAMESPACE

constexpr auto with_sizes( const auto &that, auto &&func ) requires ( requires { PI( that.size() ); } || requires { that.sizes(); } ) {
    if constexpr ( requires { that.sizes(); } )
        return func( that.sizes() );
    else {
        PI size = that.size();
        return func( Span<PI,1>{ &size } );
    }
}

END_VFS_NAMESPACE
