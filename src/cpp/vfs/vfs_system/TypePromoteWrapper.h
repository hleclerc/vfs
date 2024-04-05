#pragma once

#include "../support/CtStringValue.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<CtStringValue name,class... Types> struct TypePromoteWrapper;

/// by default...
template<CtStringValue name,class Type> struct TypePromoteWrapper<name,Type,Type> { using value = Type; };
template<CtStringValue name,class Type> struct TypePromoteWrapper<name,Type> { using value = Type; };

END_VFS_INTERNAL_NAMESPACE
