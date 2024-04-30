#pragma once

#include "common_macros.h"

BEG_VFS_NAMESPACE

auto on_wrapped_value( auto &&object, auto &&func ) requires requires { object.wrapped_value(); } { return func( std::move( object.wrapped_value() ) ); }
auto on_wrapped_value( auto &object, auto &&func ) requires requires { object.wrapped_value(); } { return func( object.wrapped_value() ); }

END_VFS_NAMESPACE
