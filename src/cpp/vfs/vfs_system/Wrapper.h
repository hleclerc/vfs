#pragma once

#include "../support/Tuple.h"

BEG_VFS_INTERNAL_NAMESPACE

/// base class for objects that wrap "real" data, with a wrapper_key() method to find which is the type.
struct Wrapper {};

// concept + type test
TT concept IsAWrapper = std::is_base_of_v<Wrapper,std::decay_t<T>>;
TT struct IsAWrapper_struct { enum { value = IsAWrapper<T> }; };

//
template<class T> Tuple<> wrapper_keys( const T & ) requires ( ! IsAWrapper<T> ) { return {}; }

END_VFS_INTERNAL_NAMESPACE
