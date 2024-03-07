#pragma once

#include "STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "../vfs_system/VfsArg.h" // IWYU pragma: export
#include "tensor_order.h"
#include "call_by_name.h"
#include "type_promote.h"
#include "CtType.h"
#include <cmath>

BEG_VFS_NAMESPACE

// needed declarationsn defined elsewhere
auto make_ArrayImpl_from_binary_operations( auto op_name, auto &&a, auto &&b ); //defined in make_ArrayImpl_from_binary_operations.h
auto make_ArrayImpl_from_unary_operations( auto op_name, auto &&a ); //defined in make_ArrayImpl_from_unary_operations.h
Ti constexpr auto ct_value_wrapper_for(); // defined in CtInt.h

//
#define DEFAULT_BIN_OPERATOR_CODE( NAME ) \
    /* methods */ \
    if constexpr( requires { a.NAME( FORWARD( b ) ); } ) { \
        return a.NAME( FORWARD( b ) ); \
    } else \
    \
    /* virtual */ \
    if constexpr( VfsArg<DECAYED_TYPE_OF( a )> || VfsArg<DECAYED_TYPE_OF( b )> ) { \
        using Ret = VALUE_IN_DECAYED_TYPE_OF( type_promote( CtString<#NAME>(), CT_DECAYED_TYPE_OF( a ), CT_DECAYED_TYPE_OF( b ) ) ); \
        return vfs_call< #NAME, CtStringList<>, Ret >( FORWARD( a ), FORWARD( b ) ); \
    } else \
    \
    /* ct value */ \
    if constexpr( requires { std::decay_t<decltype( a )>::ct_value(); std::decay_t<decltype( b )>::ct_value(); } ) { \
        constexpr auto val = call_by_name( CtString<#NAME>(), std::decay_t<decltype( a )>::ct_value(), std::decay_t<decltype( b )>::ct_value() ); \
        return ct_value_wrapper_for<val>(); \
    } else \
    \
    if constexpr( requires { std::decay_t<decltype( a )>::ct_value(); } ) { \
        return call_by_name( CtString<#NAME>(), std::decay_t<decltype( a )>::ct_value(), FORWARD( b ) ); \
    } else \
    \
    if constexpr( requires { std::decay_t<decltype( b )>::ct_value(); } ) { \
        return call_by_name( CtString<#NAME>(), FORWARD( a ), std::decay_t<decltype( b )>::ct_value() ); \
    } else \
\
    /* arrays */ \
    if constexpr( tensor_order( CT_DECAYED_TYPE_OF( a ) ) || tensor_order( CT_DECAYED_TYPE_OF( b ) ) ) { \
        return make_ArrayImpl_from_binary_operations( CtString<#NAME>(), FORWARD( a ), FORWARD( b ) ); \
    } else \


#define DEFAULT_UNA_OPERATOR_CODE( NAME ) \
    /* methods */ \
    if constexpr( requires { a.NAME(); } ) { \
        return a.NAME(); \
    } else \
    \
    /* virtual */ \
    if constexpr( requires { a.template _real_type_call<#NAME>( FORWARD( a ) ); } ) { \
        return a.template _real_type_call<#NAME>( FORWARD( a ) ); \
    } else \
    \
    /* ct value */ \
    if constexpr( requires { std::decay_t<decltype( a )>::ct_value(); } ) { \
        constexpr auto val = call_by_name( CtString<#NAME>(), std::decay_t<decltype( a )>::ct_value() ); \
        return ct_value_wrapper_for<val>(); \
    } else \
    \
    /* arrays */ \
    if constexpr( tensor_order( CT_DECAYED_TYPE_OF( a ) ) ) { \
        return make_ArrayImpl_from_unary_operations( CtString<#NAME>(), FORWARD( a ) ); \
    } else \


template<class T>
constexpr bool is_always_zero( CtType<T> ) {
    if constexpr( requires { T::ct_value(); } ) {
        if constexpr( T::ct_value() == 0 )
            return true;
        else
            return false;
    } else
        return false;
}

template<class T>
constexpr bool is_scalar( CtType<T> t ) {
    return is_always_zero( CT_DECAYED_TYPE_OF( tensor_order( t ) ) );
}

constexpr auto add( auto &&a, auto &&b ) {
    // 0 + ..., ... + 0
    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( a ) ) ) {
        return FORWARD( b );
    } else

    if constexpr( is_always_zero( CT_DECAYED_TYPE_OF( b ) ) ) {
        return FORWARD( a );
    } else

    // Str concatenation
    if constexpr( requires{ StrView( a ); StrView( b ); } ) {
        std::string_view sa( a );
        std::string_view sb( b );

        std::string res;
        res.resize( sa.size() + sb.size() );
        std::copy( sa.begin(), sa.end(), res.data() );
        std::copy( sb.begin(), sb.end(), res.data() + sa.size() );
        return res;
    } else

    DEFAULT_BIN_OPERATOR_CODE( add )

    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "found no way to call add" );
}

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

// ---------------------------------------------------------------------------------------------------------------

///
struct WithDefaultOperators {
    #define ADD_BIN_OP( OP, FUN ) friend constexpr auto OP( auto &&a,  auto &&b ) requires  \
        std::is_base_of_v<WithDefaultOperators,DECAYED_TYPE_OF(a)> || \
        std::is_base_of_v<WithDefaultOperators,DECAYED_TYPE_OF(b)> { return FUN( FORWARD( a ), FORWARD( b ) ); }
    #define ADD_UNA_OP( OP, FUN ) friend constexpr auto OP( auto &&a ) requires  \
        std::is_base_of_v<WithDefaultOperators,DECAYED_TYPE_OF(a)> { return FUN( FORWARD( a ) ); }

    ADD_BIN_OP( operator+ , add )
    ADD_BIN_OP( operator- , sub )
    ADD_BIN_OP( operator* , mul )
    ADD_BIN_OP( operator/ , div )
    ADD_BIN_OP( operator% , mod )
    ADD_BIN_OP( operator< , inf )
    ADD_BIN_OP( operator> , sup )
    ADD_BIN_OP( operator<=, leq )
    ADD_BIN_OP( operator>=, geq )
    ADD_BIN_OP( operator==, equ )
    ADD_BIN_OP( operator!=, neq )

    ADD_UNA_OP( operator- , neg )

    friend auto &operator+=( auto &a, auto &&b ) { a = a + FORWARD( b ); return a; }
    friend auto &operator/=( auto &a, auto &&b ) { a = a / FORWARD( b ); return a; }

    #undef ADD_BIN_OP
    #undef ADD_UNA_OP
};

END_VFS_NAMESPACE

