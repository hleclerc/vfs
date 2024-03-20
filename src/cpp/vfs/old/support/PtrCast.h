#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

template<class PtrType,class TargetRef>
struct PtrCast;

template<class V,class TargetRef>
struct PtrCast<V *,TargetRef> {
    using type = TargetRef *;
};

template<class V,class TargetRef>
struct PtrCast<UniquePtr<V>,TargetRef> {
    using type = UniquePtr<TargetRef>;
};

END_VFS_NAMESPACE
