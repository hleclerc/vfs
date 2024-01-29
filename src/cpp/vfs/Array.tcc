#pragma once

#include "Array.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item,class Tags>
#define UTP Array<Item,Tags>

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values ) : Array(
        FromTypeAndCtorArguments(), array_type_for_ctor_args( CtType<Item>(), Tags(), values ), values ) {
}

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<T>> &values ) : Array(
        FromTypeAndCtorArguments(), array_type_for_ctor_args( CtType<Item>(), Tags(), values ), values ) {
}

DTP TT UTP::Array( const std::initializer_list<T> &values ) : Array(
        FromTypeAndCtorArguments(), array_type_for_ctor_args( CtType<Item>(), Tags(), values ), values ) {
}

DTP TA UTP::Array( A &&...args ) requires Has_array_type_for_ctor_args<Item,Tags,A...> : Array(
        FromTypeAndCtorArguments(), array_type_for_ctor_args( CtType<Item>(), args... ), FORWARD( args )... ) {
}

DTP Int UTP::size() const {
    return VFS_CALL_METHOD( size, Int, *this );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
