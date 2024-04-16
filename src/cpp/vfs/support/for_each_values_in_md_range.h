#pragma once

#include "for_each_value_in_range.h"
#include "Tuple.h"

BEG_VFS_NAMESPACE 

template<class Head,class...Tail> void for_each_values_in_md_range( auto &&func, const Tuple<Head,Tail...> &t, auto &&...values_so_far ) {
    for_each_value_in_range( [&]( auto &&new_value ) {
        for_each_values_in_md_range( func, t.tail, values_so_far..., new_value );
    }, t.head );
}
void for_each_values_in_md_range( auto &&func, const Tuple<> &t, auto &&...values_so_far ) {
    func( values_so_far... );
}

END_VFS_NAMESPACE