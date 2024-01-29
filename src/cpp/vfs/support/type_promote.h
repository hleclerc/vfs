/// to get the expected return type of a VFS_CALL
#pragma once

#include "type_promote_scalar.h"
#include "tensor_order.h"
#include "item_type.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue op,class T_A>
constexpr auto type_promote( CtString<op>, CtType<T_A> a ) {
    return a;
}

///
template<CtStringValue op,class T_A,class T_B>
constexpr auto type_promote( CtString<op> o, CtType<T_A> a, CtType<T_B> b ) {
    // tensor_order( a ), tensor_order( b ) == 0
    if constexpr ( tensor_order( a ).value && tensor_order( b ).value == 0 ) {
        return array_like_with_item_type( a, type_promote( o, item_type( a ), b ) );
    } else

    // tensor_order( a ) == 0, tensor_order( b )
    if constexpr ( tensor_order( a ).value == 0 && tensor_order( b ).value ) {
        return array_like_with_item_type( b, type_promote( o, a, item_type( b ) ) );
    } else

    // tensor_order( a ) > tensor_order( b ), both arrays
        if constexpr ( tensor_order( a ).value > tensor_order( b ).value ) {
        return array_like_with_item_type( a, type_promote( o, item_type( a ), item_type( b ) ) );
    } else

    // tensor_order( a ) < tensor_order( b ), both arrays
    if constexpr ( tensor_order( a ).value < tensor_order( b ).value ) {
        return array_like_with_item_type( b, type_promote( o, item_type( a ), item_type( b ) ) );
    } else

    // tensor_order( a ) == tensor_order( b ), both arrays
    if constexpr ( tensor_order( a ).value == tensor_order( b ).value && tensor_order( a ).value ) {
        // TODO: find best tensor type
        return array_like_with_item_type( a, type_promote( o, item_type( a ), item_type( b ) ) );
    } else

    // TODO: special cases
    return type_promote_scalar( o, a, b );
}

END_VFS_NAMESPACE
