#pragma once

#include "CtStringList.h"
#include "CtTypeList.h"
#include "CtIntList.h"
#include "CtString.h"
#include "CtType.h"
#include "CtInt.h"

BEG_VFS_NAMESPACE

template<CtStringValue... str> constexpr auto ct_known( CtStringList<str...> ) { return CtInt<1>(); }
template<class... T> constexpr auto ct_known( CtTypeList<T...> ) { return CtInt<1>(); }
template<int... i> constexpr auto ct_known( CtIntList<i...> ) { return CtInt<1>(); }

template<CtStringValue str> constexpr auto ct_known( CtString<str> ) { return CtInt<1>(); }
template<class T> constexpr auto ct_known( CtType<T> ) { return CtInt<1>(); }
template<int i> constexpr auto ct_known( CtInt<i> ) { return CtInt<1>(); }

constexpr auto ct_known( const auto & ) { return CtInt<0>(); }

END_VFS_NAMESPACE
