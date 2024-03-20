#pragma once

#include "../TypeConfig.h"

BEG_VFS_NAMESPACE

inline Str ctor_for( SI32 c ) { return std::to_string( c ); }

Str ctor_for( const auto &vec ) {
    if constexpr ( requires{ Str( vec ); } ) {
        Str res = "\"";
        for( char c : Str( vec ) ) {
            if ( c == '\\' ) { res += "\\\\"; continue; }
            if ( c == '\n' ) { res += "\\n"; continue; }
            if ( c == '"' ) { res += "\\\""; continue; }
            res += c;
        }
        res += "\"";
        return res;
    } else if constexpr ( requires{ std::begin( vec ); } ) {
        if ( std::begin( vec ) == std::end( vec ) )
            return "{}";
        Str res = "{ ";
        for( const auto &v : vec ) {
            if ( res.size() > 2 )
                res += ", ";
            res += ctor_for( v );
        }
        return res + " }";
    } else
        STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "" );
}

END_VFS_NAMESPACE
