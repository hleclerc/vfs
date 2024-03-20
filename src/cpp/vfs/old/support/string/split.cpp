#include "split.h"

BEG_VFS_NAMESPACE
    
        Vec<Str> split( StrView str, const Str &sep ) {
            Vec<Str> res;

    while ( true ) {
        auto p = str.find( sep );
        if ( p == str.npos ) {
            res.push_back( Str{ str } );
            break;
        }

        res.push_back( Str{ str.substr( 0, p ) } );
        str.remove_prefix( p + sep.size() );
    }

    return res;
}

}
