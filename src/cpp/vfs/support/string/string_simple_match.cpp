#include "string_simple_match.h"

BEG_VFS_NAMESPACE

bool string_simple_match( StrView str, StrView pattern ) {
    if ( pattern.starts_with( "*" ) ) {
        if ( pattern.size() == 1 )
            return true;
        if ( pattern.ends_with( "*" ) )
            return str.find( pattern.substr( 1, pattern.size() - 2 ) ) != str.npos;
        return str.ends_with( pattern.substr( 1 ) );
    }
    if ( pattern.ends_with( "*" ) )
        return str.starts_with( pattern.substr( 0, pattern.size() - 1 ) );
    return str == pattern;
}

END_VFS_NAMESPACE
