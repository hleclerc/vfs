#include "ctor_of_string_to_string.h"

namespace Vfs {

Str ctor_of_string_to_string( StrView s ) {
    ASSERT( s.size() && s[ 0 ] == '"' );
    ASSERT( s.back() == '"' );
    s.remove_suffix( 1 );
    s.remove_prefix( 1 );

    Str res;
    for( PI i = 0; i < s.size(); ++i ) {
        if ( s[ i ] == '\\' && i + 1 < s.size() ) {
            res += s[ ++i ];
            continue;
        }
        res += s[ i ];
    }
    return res;
}

}
