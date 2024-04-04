#pragma once

#include "TdType.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class Impl>
class TdType_ : public TdType<typename Impl::FinalWrapper> {
public:
    virtual Str  cast_type    () const override { return type_name<Impl>(); }
};

END_VFS_INTERNAL_NAMESPACE
