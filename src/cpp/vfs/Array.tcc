#pragma once

#include "Array.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item,class Tags>
#define UTP Array<Item,Tags>

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values ) {
    VFS_CALL_METHOD_DINK( construct, void, _vfs_type_and_data, FromTypeAndCtorArguments(), vfs_td_impl_type( CtType<Array>(), values ), values );
}

DTP TT UTP::Array( const std::initializer_list<std::initializer_list<T>> &values ) {
    VFS_CALL_METHOD_DINK( construct, void, _vfs_type_and_data, FromTypeAndCtorArguments(), vfs_td_impl_type( CtType<Array>(), values ), values );
}

DTP TT UTP::Array( const std::initializer_list<T> &values ) {
    VFS_CALL_METHOD_DINK( construct, void, _vfs_type_and_data, FromTypeAndCtorArguments(), vfs_td_impl_type( CtType<Array>(), values ), values );
}

DTP Int UTP::size() const {
    return VFS_CALL_METHOD( size, Int, *this );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
