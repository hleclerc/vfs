#pragma once

#include "../support/CtStringValue.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue name,class... Types> struct VfsTypePromoteWrapper;

/// by default...
template<CtStringValue name,class Type> struct VfsTypePromoteWrapper<name,Type,Type> { using value = Type; };
template<CtStringValue name,class Type> struct VfsTypePromoteWrapper<name,Type> { using value = Type; };

END_VFS_NAMESPACE
