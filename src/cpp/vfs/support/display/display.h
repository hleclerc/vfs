#pragma once

#include "../string/read_arg_name.h"
#include "Displayer.h"

BEG_VFS_NAMESPACE

DisplayItem *display( Displayer &ds, std::string_view   str );
DisplayItem *display( Displayer &ds, const std::string& str );
DisplayItem *display( Displayer &ds, const char*        str );
DisplayItem *display( Displayer &ds, char               str );

DisplayItem *display( Displayer &ds, std::uint64_t      val );
DisplayItem *display( Displayer &ds, std::uint32_t      val );
DisplayItem *display( Displayer &ds, std::uint16_t      val );
DisplayItem *display( Displayer &ds, std::uint8_t       val );
DisplayItem *display( Displayer &ds, std::int64_t       val );
DisplayItem *display( Displayer &ds, std::int32_t       val );
DisplayItem *display( Displayer &ds, std::int16_t       val );
DisplayItem *display( Displayer &ds, std::int8_t        val );
DisplayItem *display( Displayer &ds, bool               val );

DisplayItem *display( Displayer &ds, double             val );
DisplayItem *display( Displayer &ds, float              val );

DisplayItem *display( Displayer &ds, const void*        val );
DisplayItem *display( Displayer &ds, void*              val );

// pair
template<class A,class B>
DisplayItem *display( Displayer &ds, const std::pair<A,B> &value ) {
    return ds.array( [&]( const auto &cb ) {
        cb( display( ds, value.first ) );
        cb( display( ds, value.second ) );
    } );
}

// tuple
template<class... A>
DisplayItem *display( Displayer &ds, const std::tuple<A...> &value ) {
    return std::apply( [&]( const auto &...args ) { return ds.array( { display( ds, args )... } ); }, value );
}

// generic
template<class Ds,class T>
auto display( Ds &ds, const T &value ) {
    // value.display
    if constexpr( requires { value.display( ds ); } ) {
        return value.display( ds );
    }
    // value.to_string
    else if constexpr( requires { value.to_string(); } ) {
        return ds.string( value.to_string() );
    }
    // array (with begin/end)
    else if constexpr( requires { std::begin( value ); std::end( value ); } ) {
        std::vector<DisplayItem *> items;
        for( const auto &v : value )
            items.push_back( display( ds, v ) );
        return ds.array( items );
    }
    // array (with size and operator[])
    else if constexpr( requires { value.size(); value[ 0ul ]; } ) {
        std::vector<DisplayItem *> items;
        for( std::size_t i = 0; i < std::size_t( value.size() ); ++i )
            items.push_back( display( ds, value[ i ] ) );
        return ds.array( items );
    }
    // *value
    else if constexpr( requires { bool( value ), *value; } ) {
        if ( value )
            return display( ds, *value );
        return ds.string( "NULL" );
    }
    // T::template_type_name() (for empty structures)
    else if constexpr( requires { T::template_type_name(); } ) {
        return ds.string( T::template_type_name() );
    }
    // T::type_name() (for empty structures)
    else if constexpr( requires { T::type_name(); } ) {
        return ds.string( T::type_name() );
    }
    // value.display again (to get an error message)
    else
        return value.display( ds );
}

// DS_OBJECT
template<class...A>
DisplayItem *_ds_object( Displayer &ds, std::string_view name, std::string_view attr_names, const A &...args ) {
    return ds.object( name, { std::pair<std::string,DisplayItem *>{ read_arg_name( attr_names ), display( ds, args ) }... } );
}

#define DS_OBJECT( NAME, ... ) \
    _ds_object( ds, #NAME, #__VA_ARGS__, __VA_ARGS__ )

END_VFS_NAMESPACE
