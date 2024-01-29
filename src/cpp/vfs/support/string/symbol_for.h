#pragma once

#include "../TypeConfig.h"

BEG_VFS_NAMESPACE

inline Str symbol_for( const Str &str ) {
    Str res;
    for( char c : str ) {
        if ( c >= 'a' && c <= 'z' ) { res += c; continue; }
        if ( c >= 'A' && c <= 'Z' ) { res += c; continue; }
        if ( c >= '0' && c <= '1' ) { res += c; continue; }
        if ( c == '_' ) { res += "__"; continue; }

        if ( c == ' ' ) { res += "_s"; continue; }
        if ( c == ':' ) { res += "_d"; continue; }

        if ( c == '+' ) { res += "_P"; continue; }
        if ( c == '-' ) { res += "_S"; continue; }
        if ( c == '*' ) { res += "_M"; continue; }
        if ( c == '/' ) { res += "_D"; continue; }
        if ( c == '%' ) { res += "_O"; continue; }
        TODO;
    }
    return res;
}

END_VFS_NAMESPACE
