#pragma once

#include "../support/CtInt.h"

BEG_VFS_NAMESPACE

auto select_with_n_indices( auto &&array, CtInt<0>, auto... ) { return array; }
auto select_with_n_indices( auto &&array, CtInt<1>, auto &&i0, auto... ) { return array[ FORWARD( i0 ) ]; }
auto select_with_n_indices( auto &&array, CtInt<2>, auto &&i0, auto &&i1, auto... ) { return array( FORWARD( i0 ), FORWARD( i1 ) ); }

END_VFS_NAMESPACE
