#pragma once

#include "make_array_from_binary_operations.h" // IWYU pragma: export
#include "make_array_from_unary_operations.h" // IWYU pragma: export

#include "../STATIC_ASSERT_IN_IF_CONSTEXPR.h" // IWYU pragma: export
#include "../CtString.h" // IWYU pragma: export
#include "../VoidFunc.h" // IWYU pragma: export
#include "../CtType.h" // IWYU pragma: export
#include "../get.h" // IWYU pragma: export
#include "../set.h" // IWYU pragma: export

#include "../../vfs_system/TypePromoteWrapper.h" // IWYU pragma: export
#include "../../vfs_system/WrapperTypeFor.h" // IWYU pragma: export

#include <string_view> // IWYU pragma: export
#include <string> // IWYU pragma: export
#include <cmath> // IWYU pragma: export

BEG_VFS_NAMESPACE
struct VfsWrapper;

// needed declarationsn defined elsewhere
Ti constexpr auto ct_value_wrapper_for(); // defined in CtInt.h

//
#define DEFAULT_BIN_OPERATOR_CODE_SIGN( NAME, SIGN ) \
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
        constexpr auto val = DECAYED_TYPE_OF( a )::ct_value() SIGN DECAYED_TYPE_OF( b )::ct_value(); \
        return ct_value_wrapper_for<val>(); \
    } else \
    \
    if constexpr( requires { DECAYED_TYPE_OF( a )::ct_value(); } ) { \
        return DECAYED_TYPE_OF( a )::ct_value() SIGN FORWARD( b ); \
    } else \
    \
    if constexpr( requires { DECAYED_TYPE_OF( b )::ct_value(); } ) { \
        return FORWARD( a ) SIGN DECAYED_TYPE_OF( b )::ct_value(); \
    } else \
\
    /* wrapper */ \
    if constexpr( std::is_base_of_v<VfsWrapper,DECAYED_TYPE_OF( a )> || std::is_base_of_v<VfsWrapper,DECAYED_TYPE_OF( b )> ) { \
        using Wta = VfsWrapperTypeFor<DECAYED_TYPE_OF( a )>::value; \
        using Wtb = VfsWrapperTypeFor<DECAYED_TYPE_OF( b )>::value; \
        using Res = VfsTypePromoteWrapper<#NAME,Wta,Wtb>::value; \
        return a.template __wrapper_call<Res>( CtString<#NAME "_pmt__method">(), a, b ); \
    } else \
    \
    /* get( ... ) */ \
    if constexpr( requires { get( FORWARD( a ) ); } ) { \
        return get( FORWARD( a ) ) SIGN FORWARD( b ); \
    } else \
    \
    if constexpr( requires { get( FORWARD( b ) ); } ) { \
        return FORWARD( a ) SIGN get( FORWARD( b ) ); \
    } else \
    \
    /* arrays */ \
    if constexpr( TensorOrder<DECAYED_TYPE_OF( a )>::value || TensorOrder<DECAYED_TYPE_OF( b )>::value ) { \
        return make_array_from_binary_operations( Functor_##NAME(), FORWARD( a ), FORWARD( b ) ); \
    } else \

// sign means operator like +, *, ... which have to be place between the operands
#define DEFAULT_BIN_SELF_OPERATOR_CODE_SIGN( NAME, SELF_SIGN, SIGN ) \
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
    if constexpr( requires { DECAYED_TYPE_OF( b )::ct_value(); } ) { \
        FORWARD( a ) SELF_SIGN DECAYED_TYPE_OF( b )::ct_value(); \
        return; \
    } else \
    \
    /* wrapper */ \
    if constexpr( std::is_base_of_v<VfsWrapper,DECAYED_TYPE_OF( a )> || std::is_base_of_v<VfsWrapper,DECAYED_TYPE_OF( b )> ) { \
        using Wta = VfsWrapperTypeFor<DECAYED_TYPE_OF( a )>::value; \
        using Wtb = VfsWrapperTypeFor<DECAYED_TYPE_OF( b )>::value; \
        using Res = VfsTypePromoteWrapper<#NAME,Wta,Wtb>::value; \
        a.template __wrapper_call<void>( CtString<"self_op_pmt__method">(), a, VoidFunc(), Functor_##NAME(), b ); \
        return; \
    } else \
    \
    /* get( ... ) */ \
    if constexpr( requires { get( FORWARD( b ) ); } ) { \
        FORWARD( a ) SELF_SIGN get( FORWARD( b ) ); \
        return; \
    } else \
    \
    if constexpr( requires { set( a, get( a ) SIGN FORWARD( b ) ); } ) { \
        set( a, get( a ) SIGN FORWARD( b ) ); \
        return; \
    } else \
    \

    // /* arrays */ \
    // if constexpr( TensorOrder<DECAYED_TYPE_OF( a )>::value || TensorOrder<DECAYED_TYPE_OF( b )>::value ) { \
    //     return make_array_from_binary_operations( Functor_##NAME(), FORWARD( a ), FORWARD( b ) ); \
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
\
    /* arrays */ \
    if constexpr( TensorOrder<DECAYED_TYPE_OF( a )>::value || TensorOrder<DECAYED_TYPE_OF( b )>::value ) { \
        return make_array_from_binary_operations( Functor_##NAME(), FORWARD( a ), FORWARD( b ) ); \
    } else \


#define DEFAULT_UNA_OPERATOR_CODE( NAME, FUNC ) \
    /* methods */ \
    if constexpr( requires { a.NAME(); } ) { \
        return a.NAME(); \
    } else \
    \
    /* ct value */ \
    if constexpr( requires { DECAYED_TYPE_OF( a )::ct_value(); } ) { \
        constexpr auto val = FUNC( DECAYED_TYPE_OF( a )::ct_value() ); \
        return ct_value_wrapper_for<val>(); \
    } else \
    \
    /* arrays */ \
    if constexpr( TensorOrder<DECAYED_TYPE_OF( a )>::value ) { \
        return make_array_from_unary_operations( Functor_##NAME(), FORWARD( a ) ); \
    } else \
 \

END_VFS_NAMESPACE

