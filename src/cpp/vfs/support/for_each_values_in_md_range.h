#pragma once

#include "for_each_value_in_range.h"
#include "Tup.h"

BEG_VFS_NAMESPACE

void for_each_values_in_md_range_with_offset_index( auto &&func, auto &&list, auto offset_index, auto &&...values_so_far ) {
    // TODO: add contexpr when possible (to avoid infinite recursion)
    if ( offset_index < list.size() ) {
        for_each_value_in_range( [&]( auto &&new_value ) {
            for_each_values_in_md_range_with_offset_index( func, list, offset_index + CtInt<1>(), values_so_far..., new_value );
        }, list[ offset_index ] );
    } else
        func( FORWARD( values_so_far )... );
}

void for_each_values_in_md_range( auto &&func, auto &&list, auto &&...values_so_far ) {
    // Tuple<Head,Tail...>
    if constexpr ( requires { list.head; list.tail; } ) {
        for_each_value_in_range( [&]( auto &&new_value ) {
            for_each_values_in_md_range( func, list.tail, values_so_far..., new_value );
        }, list.head );
    } else

    // Tuple<>
    if constexpr ( std::is_same_v<DECAYED_TYPE_OF( list ),Tup<>> ) {
        func( FORWARD( values_so_far )... );
    } else

    // .size, [ ... ]
    if constexpr ( requires { list.size(); list[ 0 ]; } ) {
        for_each_values_in_md_range_with_offset_index( func, list, list.size() * CtInt<0>(), FORWARD( values_so_far )... );
    } else

    //
    STATIC_ASSERT_IN_IF_CONSTEXPR( false, "..." );
}

END_VFS_NAMESPACE
