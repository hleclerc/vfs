#pragma once

#include "common_macros.h"
#include "CtString.h"

namespace Vfs {

// needed declarations, defined elsewhere
constexpr auto min( auto &&a, auto &&b );
constexpr auto max( auto &&a, auto &&b );

//
auto exact_div( auto &&a, auto &&b ) requires ( std::is_integral_v<DECAYED_TYPE_OF(a)> == false || std::is_integral_v<DECAYED_TYPE_OF(b)> == false ) {
    return FORWARD( a ) / FORWARD( b );
}

// operators
constexpr auto call_by_name( CtString<"add">, auto &&a, auto &&b ) { return FORWARD( a ) +  FORWARD( b ); }
constexpr auto call_by_name( CtString<"sub">, auto &&a, auto &&b ) { return FORWARD( a ) -  FORWARD( b ); }
constexpr auto call_by_name( CtString<"mul">, auto &&a, auto &&b ) { return FORWARD( a ) *  FORWARD( b ); }
constexpr auto call_by_name( CtString<"div">, auto &&a, auto &&b ) { return FORWARD( a ) /  FORWARD( b ); }
constexpr auto call_by_name( CtString<"mod">, auto &&a, auto &&b ) { return FORWARD( a ) %  FORWARD( b ); }

constexpr auto call_by_name( CtString<"inf">, auto &&a, auto &&b ) { return FORWARD( a ) <  FORWARD( b ); }
constexpr auto call_by_name( CtString<"sup">, auto &&a, auto &&b ) { return FORWARD( a ) <  FORWARD( b ); }
constexpr auto call_by_name( CtString<"leq">, auto &&a, auto &&b ) { return FORWARD( a ) <= FORWARD( b ); }
constexpr auto call_by_name( CtString<"geq">, auto &&a, auto &&b ) { return FORWARD( a ) >= FORWARD( b ); }
constexpr auto call_by_name( CtString<"equ">, auto &&a, auto &&b ) { return FORWARD( a ) == FORWARD( b ); }
constexpr auto call_by_name( CtString<"neq">, auto &&a, auto &&b ) { return FORWARD( a ) != FORWARD( b ); }

constexpr auto call_by_name( CtString<"neg">, auto &&a ) { return - FORWARD( a ); }

// functions
constexpr auto call_by_name( CtString<"min">, auto &&a, auto &&b ) { return min( FORWARD( a ), FORWARD( b ) ); }
constexpr auto call_by_name( CtString<"max">, auto &&a, auto &&b ) { return max( FORWARD( a ), FORWARD( b ) ); }

constexpr auto call_by_name( CtString<"exact_div">, auto &&a, auto &&b ) { return exact_div( FORWARD( a ), FORWARD( b ) ); }

} // namespace Vfs
