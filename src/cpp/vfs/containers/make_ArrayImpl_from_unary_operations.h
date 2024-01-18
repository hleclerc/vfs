#pragma once

#include "../support/call_by_name.h"
#include "../support/TypeConfig.h"

namespace Vfs {

auto make_ArrayImpl_from_unary_operations( auto op_name, auto &&a ) {
    using DA = DECAYED_TYPE_OF( a );
    if constexpr ( requires { a.~VecImpl(); } ) {
        auto item_type = DECAYED_CT_OF( call_by_name( op_name, a[ 0 ] ) );
        using Res = GET_DT_VALUE( VecImplType_for( item_type, a.size() ) );
        return Res( FromOperationOnItemsOf(), op_name, CtIntList<1>(), FORWARD( a ) );
    } else
        static_assert( 0, "TODO" );
}

} // namespace Vfs
