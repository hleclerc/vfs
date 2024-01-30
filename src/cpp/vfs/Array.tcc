#pragma once

#include "Array.h"

BEG_VFS_NAMESPACE


#define DTP template<class Item,class Tags>
#define UTP Array<Item,Tags>

DTP auto vfs_td_impl_type( CtType<UTP>, const auto &arg ) {
    return CT_DECAYED_TYPE_OF( arg );
}

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values ) : Array(
        FromTypeAndCtorArguments(), ArrayTagListAnalyzer::array_type_for_ctor_args( CtType<Item>(), Tags(), values ), values ) {
    P( __LINE__ );
}

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<T>> &values ) : Array(
        FromTypeAndCtorArguments(), ArrayTagListAnalyzer::array_type_for_ctor_args( CtType<Item>(), Tags(), values ), values ) {
    P( __LINE__ );
}

DTP TT UTP::Array( const std::initializer_list<T> &values ) : Array(
        FromTypeAndCtorArguments(), ArrayTagListAnalyzer::array_type_for_ctor_args( CtType<Item>(), Tags(), values ), values ) {
    P( __LINE__ );
}

DTP TA UTP::Array( A &&...args ) requires Has_array_type_for_ctor_args<Tags,Item,A...> : Array(
        FromTypeAndCtorArguments(), ArrayTagListAnalyzer::array_type_for_ctor_args( CtType<Item>(), Tags(), args... ), FORWARD( args )... ) {
    P( __LINE__ );
}

DTP Int UTP::size() const {
    return VFS_CALL_METHOD( size, Int, *this );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
