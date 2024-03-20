#pragma once

#include "common_macros.h"

BEG_VFS_NAMESPACE

/// return the first value that gives bool( value ) = true. In none, return default_value
auto or_with_return( auto &&default_value, auto &&head, auto &&...tail ) {
    if ( head )
        return FORWARD( head );
    return or_with_return( FORWARD( default_value ), FORWARD( tail )... );
}

auto or_with_return( auto &&default_value ) {
    return FORWARD( default_value );
}

END_VFS_NAMESPACE
