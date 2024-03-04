#pragma once

#include "display/display.h"
#include <iostream>
#include <mutex>

BEG_VFS_NAMESPACE
template<class Value> std::string type_name();

template<class OS,class... ArgValues>
void __print_with_mutex( OS &os, const DisplayParameters &dp, std::string_view arg_names, ArgValues &&...arg_values ) {
    Displayer ds;
    DisplayItem *obj = ds.object( "", { std::pair<std::string,DisplayItem *>{ read_arg_name( arg_names ), display( ds, std::forward<ArgValues>( arg_values ) ) }... } );

    static std::mutex m;
    m.lock();
    obj->write_to( os, dp );
    os << std::endl;
    m.unlock();
}

template<class OS,class... ArgValues>
void __print_types_with_mutex( OS &os, const DisplayParameters &dp, std::string_view arg_names, ArgValues &&...arg_values ) {
    __print_with_mutex( os, dp, arg_names, type_name<decltype(arg_values)>()... );
}

//template<class... ArgValues>
//void __show( const DisplayParms &dp, const Str &arg_names, ArgValues &&...arg_values ) {
//    DisplayGraph dg( arg_names, Vec<DisplayNode>::from_values( std::forward<ArgValues>( arg_values )... ) );
//    dg.show( dp );
//}

#ifndef P
    // PRINT in cout
    #define P( ... ) \
        VFS_NAMESPACE::__print_with_mutex( std::cout, VFS_NAMESPACE::DisplayParameters::for_debug_info(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT in cerr
    #define PE( ... ) \
        VFS_NAMESPACE::__print_with_mutex( std::cerr, VFS_NAMESPACE::DisplayParameters::for_debug_info(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT in cout with options
    #define PO( VALUE, PARAMS ) \
        __print_with_mutex( std::cout, " -> ", ", ", PARAMS, #VALUE, VALUE )

    // PRINT in cout
    #define PT( ... ) \
        VFS_NAMESPACE::__print_types_with_mutex( std::cout, VFS_NAMESPACE::DisplayParameters::for_debug_info(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT with .display in cout with options
    #define PD( VALUE, ... ) \
        ( VALUE ).display( __VA_ARGS__ ).display( std::cout  << #VALUE " -> \n" ) << std::endl

    // PRINT with file and line info
    #define PM( ... ) \
        __print_with_mutex( std::cout << __FILE__ << ':' << __LINE__, " -> ", ", ", {}, #__VA_ARGS__, __VA_ARGS__, WithSep{""},  )

    // Display graph
    #define SHOW( ... ) \
        __show( {}, #__VA_ARGS__, __VA_ARGS__ )

    // PRINT counter
    #define PC do { static int cpt = 0; PE( cpt++ ); } while ( false )
#endif

END_VFS_NAMESPACE
