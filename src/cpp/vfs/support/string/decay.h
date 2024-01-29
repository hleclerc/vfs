#pragma once

#include "../TypeConfig.h"

BEG_VFS_NAMESPACE

inline StrView decay( StrView type ) {
    while ( type.ends_with( '&' ) || type.ends_with( ' ' ) )
        type.remove_suffix( 1 );
    if ( type.starts_with( "const " ) )
        type.remove_prefix( 6 );
    while ( type.starts_with( ' ' ) )
        type.remove_suffix( 1 );
    return type;
}

END_VFS_NAMESPACE
