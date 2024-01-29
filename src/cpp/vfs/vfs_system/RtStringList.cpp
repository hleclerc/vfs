#include "../support/string/ctor_for.h"
#include "../support/string/join.h"
#include "RtStringList.h"

BEG_VFS_NAMESPACE

Vec<Str> RtStringList::cast_to_string( StrView str ) {
    Vec<Str> res;
    PI i = str.find( "CtStringList<" ) + 13;
    for( ; i < str.size(); ++i ) {
        if ( str[ i ] == '{' || str[ i ] == '}' || str[ i ] == ' ' || str[ i ] == ',' )
            continue;

        if ( str[ i ] == '"' ) {
            PI b = ++i;
            for( ; i < str.size(); ++i ) {
                if ( str[ i ] == '\\' && i + 1 < str.size() ) {
                    ++i;
                    continue;
                }
                if ( str[ i ] == '"' )
                    break;
            }
            res.push_back( Str{ str.begin() + b, str.begin() + i } );
        }
    }

    return res;
}

Vec<Str> vfs_object_ct_cast( const RtStringList &obj ) {
    Str res = "auto {ARG} = CtStringList<";
    res += join_map( obj.value, ctor_for<Str> );
    return { res + ">();" };
}

const Vec<Str> &vfs_object_ct_key( const RtStringList &obj ) {
    return obj.value;
}



END_VFS_NAMESPACE
