#pragma once

//#include "../containers/Vec.h"
#include "to_string.h"

BEG_VFS_NAMESPACE

template<class T> std::string join( const T &vec, std::string sep = ", " ) {
    std::string res;
    for( const auto &v : vec ) {
        if ( res.size() )
            res += sep;
        res += to_string( v );
    }
    return res;
}

template<class T> std::string join( const T &vec, std::string sep, std::string last_sep ) {
    std::string res;
    for( const auto &v : vec ) {
        if ( res.size() )
            res += &v == &vec.back() ? last_sep : sep;
        res += to_string( v );
    }
    return res;
}

template<class T,class F> std::string join_map( const T &vec, F &&func, std::string sep, std::string last_sep ) {
    std::string res;
    for( const auto &v : vec ) {
        if ( res.size() )
            res += &v == &vec.back() ? last_sep : sep;
        res += to_string( func( v ) );
    }
    return res;
}

template<class T,class F> std::string join_map( const T &vec, F &&func, std::string sep = ", " ) {
    return join_map( vec, std::forward<F>( func ), sep, sep );
}

//inline std::string join_binary( const Vec<std::string> &vec, String func = "std::max" ) {
//    ASSERT( vec.size() );

//    String res = vec[ 0 ];
//    for( PI i = 1; i < vec.size(); ++i )
//        res = func + "( " + res + ", " + vec[ i ] + " )";
//    return res;
//}

END_VFS_NAMESPACE
