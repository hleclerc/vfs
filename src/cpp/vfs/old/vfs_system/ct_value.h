#pragma once

#include "VirtualCtString.h"
#include "VirtualCtInt.h"

// #include "../Any.h"

BEG_VFS_NAMESPACE
class Any;

// common surdefs
TSs auto ct_value( CtStringList<S...> v ) { return v; }
TS auto ct_value( CtString<S> v ) { return v; }
TT auto ct_value( CtType<T> v ) { return v; }
Ti auto ct_value( CtInt<i> v ) { return v; }

// by default
auto ct_value( const auto &v ) {
    using T = decltype( v );
    if constexpr( VfsArg<T> )
        return VFS_CALL( ct_value, CtStringList<>, Any, FORWARD( v ) );
    else if constexpr( ScalarLike<T> )
        return VirtualCtInt{ int( v ) };
    else if constexpr( StrLike<T> )
        return VirtualCtString{ FORWARD( v ) };
    else
        STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( 0, "don't know how to make ct value" );
}

END_VFS_NAMESPACE
