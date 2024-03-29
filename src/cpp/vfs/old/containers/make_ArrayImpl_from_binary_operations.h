#pragma once

#include "../support/STATIC_ASSERT_IN_IF_CONSTEXPR.h"
#include "../support/call_by_name.h"
#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

auto make_ArrayImpl_from_binary_operations( auto op_name, auto &&a, auto &&b ) {
    using DA = DECAYED_TYPE_OF( a );
    using DB = DECAYED_TYPE_OF( b );

    // Vec, Vec. TODO: replace by a more generic definition
    if constexpr ( requires { a.~Vec(); b.~Vec(); } ) {
        auto item_type = CT_DECAYED_TYPE_OF( call_by_name( op_name, a[ 0 ], b[ 0 ] ) );
        using Res = VALUE_IN_DECAYED_TYPE_OF( VecType_for( item_type, min( a.size(), b.size() ) ) );
        return Res( FromOperationOnItemsOf(), op_name, CtIntList<1,1>(), FORWARD( a ), FORWARD( b ) );
    } else

    if constexpr ( requires { a.~Vec(); } && is_scalar( CtType<DB>() ) ) {
        auto item_type = CT_DECAYED_TYPE_OF( call_by_name( op_name, a[ 0 ], b ) );
        using Res = VALUE_IN_DECAYED_TYPE_OF( VecType_for( item_type, a.size() ) );
        return Res( FromOperationOnItemsOf(), op_name, CtIntList<1,0>(), FORWARD( a ), FORWARD( b ) );
    } else

    if constexpr ( is_scalar( CtType<DA>() ) && requires { b.~Vec(); } ) {
        auto item_type = CT_DECAYED_TYPE_OF( call_by_name( op_name, a, b[ 0 ] ) );
        using Res = VALUE_IN_DECAYED_TYPE_OF( VecType_for( item_type, b.size() ) );
        return Res( FromOperationOnItemsOf(), op_name, CtIntList<0,1>(), FORWARD( a ), FORWARD( b ) );
    } else

    // fail
    STATIC_ASSERT_IN_IF_CONSTEXPR( 0, "make_ArrayImpl_from_binary_operations" );
}

END_VFS_NAMESPACE

