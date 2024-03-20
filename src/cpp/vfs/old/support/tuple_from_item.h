#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

auto tuple_from_item( CtInt<0>(), auto &&tuple ) { return FORWARD( tuple ); }
auto tuple_from_item( CtInt<1>(), auto &&tuple ) { return std::apply( []( auto &&head, auto&&...tail ) { return std::make_tuple( FORWARD( tail )... ); }, tuple ); }
Ti auto tuple_from_item( CtInt<i>(), auto &&tuple ) { return tuple_from_item( CtInt<i-1>(), tuple_from_item( CtInt<1>(), FORWARD( tuple ) ) ); }

END_VFS_NAMESPACE
