#pragma once

#include "../STATIC_ASSERT_IN_IF_CONSTEXPR.h"
// #include "../vfs_system/VfsArg.h" // IWYU pragma: export
#include "../is_always_zero.h"
#include "../common_macros.h"
#include "../tensor_order.h"
#include "../CtString.h"
#include "../CtType.h"

#include <string_view>
#include <string>
#include <cmath>

BEG_VFS_NAMESPACE

// needed declarationsn defined elsewhere
auto make_ArrayImpl_from_binary_operations( auto op_name, auto &&a, auto &&b ); // defined in make_ArrayImpl_from_binary_operations.h
auto make_ArrayImpl_from_unary_operations( auto op_name, auto &&a ); // defined in make_ArrayImpl_from_unary_operations.h
Ti constexpr auto ct_value_wrapper_for(); // defined in CtInt.h

// \
// /* virtual */ \
//     if constexpr( VfsArg<DECAYED_TYPE_OF( a )> || VfsArg<DECAYED_TYPE_OF( b )> ) { \
//         using Ret = VALUE_IN_DECAYED_TYPE_OF( type_promote( CtString<#NAME>(), CT_DECAYED_TYPE_OF( a ), CT_DECAYED_TYPE_OF( b ) ) ); \
//         return vfs_call< "call_by_name", CtStringList<>, Ret >( CtString<#NAME>(), FORWARD( a ), FORWARD( b ) ); \
// } else \
//     /* arrays */ \
//     if constexpr( tensor_order( CT_DECAYED_TYPE_OF( a ) ) || tensor_order( CT_DECAYED_TYPE_OF( b ) ) ) { \
//         return make_ArrayImpl_from_binary_operations( CtString<#NAME>(), FORWARD( a ), FORWARD( b ) ); \
// } else \

//
#define DEFAULT_BIN_OPERATOR_CODE( NAME ) \
    /* methods */ \
    if constexpr( requires { a.NAME( FORWARD( b ) ); } ) { \
        return a.NAME( FORWARD( b ) ); \
    } else \
    \
    if constexpr( requires { b.r##NAME( FORWARD( a ) ); } ) { \
        return b.r##NAME( FORWARD( a ) ); \
    } else \
    \
    /* ct value */ \
    if constexpr( requires { DECAYED_TYPE_OF( a )::ct_value(); DECAYED_TYPE_OF( b )::ct_value(); } ) { \
        constexpr auto val = NAME( DECAYED_TYPE_OF( a )::ct_value(), DECAYED_TYPE_OF( b )::ct_value() ); \
        return ct_value_wrapper_for<val>(); \
    } else \
    \
    if constexpr( requires { DECAYED_TYPE_OF( a )::ct_value(); } ) { \
        return NAME( DECAYED_TYPE_OF( a )::ct_value(), FORWARD( b ) ); \
    } else \
    \
    if constexpr( requires { DECAYED_TYPE_OF( b )::ct_value(); } ) { \
        return NAME( FORWARD( a ), DECAYED_TYPE_OF( b )::ct_value() ); \
    } else \


//     /* virtual */ \
//     if constexpr( requires { a.template _real_type_call<#NAME>( FORWARD( a ) ); } ) { \
//         return a.template _real_type_call<#NAME>( FORWARD( a ) ); \
// } else \

#define DEFAULT_UNA_OPERATOR_CODE( NAME ) \
    /* methods */ \
    if constexpr( requires { a.NAME(); } ) { \
        return a.NAME(); \
    } else \
    \
    /* ct value */ \
    if constexpr( requires { DECAYED_TYPE_OF( a )::ct_value(); } ) { \
        constexpr auto val = NAME( DECAYED_TYPE_OF( a )::ct_value() ); \
        return ct_value_wrapper_for<val>(); \
    } else \
    \
    /* arrays */ \
    if constexpr( tensor_order( CT_DECAYED_TYPE_OF( a ) ) ) { \
        return make_ArrayImpl_from_unary_operations( CtString<#NAME>(), FORWARD( a ) ); \
    } else \





constexpr auto sub( auto &&a, auto &&b ) {
    // 0 - b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return - FORWARD( b );
    } else

    // a - 0
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( b ) ) ) {
        return FORWARD( a );
    } else

    DEFAULT_BIN_OPERATOR_CODE( sub )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call sub" );
}

constexpr auto mul( auto &&a, auto &&b ) {
    // 0 * b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( a );
    } else

    // a * 0
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( b ) ) ) {
        return FORWARD( b );
    } else

    DEFAULT_BIN_OPERATOR_CODE( mul )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call mul" );
}

constexpr auto div( auto &&a, auto &&b ) {
    // 0 / b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( a );
    } else

    DEFAULT_BIN_OPERATOR_CODE( div )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call div" );
}

constexpr auto mod( auto &&a, auto &&b ) {
    // 0 % b
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( a );
    } else

    DEFAULT_BIN_OPERATOR_CODE( mod )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call mod" );
}

constexpr auto inf( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( inf ) STATIC_ASSERT_IN_IF_CONSTEXPR( false, "found not way to call inf" ); }
constexpr auto sup( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( sup ) STATIC_ASSERT_IN_IF_CONSTEXPR( false, "found not way to call sup" ); }
constexpr auto leq( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( leq ) STATIC_ASSERT_IN_IF_CONSTEXPR( false, "found not way to call leq" ); }
constexpr auto geq( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( geq ) STATIC_ASSERT_IN_IF_CONSTEXPR( false, "found not way to call geq" ); }
constexpr auto equ( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( equ ) STATIC_ASSERT_IN_IF_CONSTEXPR( false, "found not way to call equ" ); }
constexpr auto neq( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( neq ) STATIC_ASSERT_IN_IF_CONSTEXPR( false, "found not way to call neq" ); }

constexpr auto min( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( min ) return a <= b ? FORWARD( a ) : FORWARD( b ); }
constexpr auto max( auto &&a, auto &&b ) { DEFAULT_BIN_OPERATOR_CODE( max ) return a >= b ? FORWARD( a ) : FORWARD( b ); }

auto neg( auto &&a ) { DEFAULT_UNA_OPERATOR_CODE( neg ) STATIC_ASSERT_IN_IF_CONSTEXPR( a, "found not way to call ..." ); }
auto abs( auto &&a ) { DEFAULT_UNA_OPERATOR_CODE( abs ) STATIC_ASSERT_IN_IF_CONSTEXPR( a, "found not way to call ..." ); }

/// scalar product
auto sp( auto &&a, auto &&b ) {
    auto res = a[ 0 ] * b[ 0 ];
    for( std::size_t i = 1; i < a.size(); ++i )
        res += a[ i ] * b[ i ];
    return res;
}

/// norm
auto norm_2_p2( auto &&a ) {
    auto res = a[ 0 ] * a[ 0 ];
    for( std::size_t i = 1; i < a.size(); ++i )
        res += a[ i ] * a[ i ];
    return res;
}

auto norm_2( auto &&a ) {
    using namespace std;
    return sqrt( norm_2_p2( FORWARD( a ) ) );
}

/// seq max
auto max( auto &&a ) requires ( tensor_order( CT_DECAYED_TYPE_OF( a ) ).always_equal( 1 ) ) {
    using std::max;
    auto res = a[ 0 ];
    for( std::size_t i = 1; i < a.size(); ++i )
        res = max( res, a[ i ] );
    return res;
}

/// seq any
bool all( auto &&a ) requires ( tensor_order( CT_DECAYED_TYPE_OF( a ) ).always_equal( 1 ) ) {
    bool res = true;
    for( std::size_t i = 0; i < a.size(); ++i )
        res &= a[ i ];
    return res;
}

/// seq argmin
std::size_t argmin( auto &&a ) requires ( tensor_order( CT_DECAYED_TYPE_OF( a ) ).always_equal( 1 ) ) {
    auto res = 0;
    for( std::size_t i = 1; i < a.size(); ++i )
        if ( a[ i ] < a[ res ] )
            res = i;
    return res;
}

/// seq argmin
std::size_t argmax( auto &&a ) requires ( tensor_order( CT_DECAYED_TYPE_OF( a ) ).always_equal( 1 ) ) {
    auto res = 0;
    for( std::size_t i = 1; i < a.size(); ++i )
        if ( a[ res ] < a[ i ] )
            res = i;
    return res;
}

END_VFS_NAMESPACE

