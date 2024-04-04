#pragma once

#include "../support/CompilationFlags.h"
#include "TdType.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class Impl>
class TdType_ : public TdType<typename Impl::FinalWrapper> {
public:
    virtual void get_compilation_flags( CompilationFlags &cg ) const { VFS_NAMESPACE::get_compilation_flags( cg, CtType<Impl>() ); }
    virtual Str  cast_type            () const override { return type_name<Impl>(); }
};

END_VFS_INTERNAL_NAMESPACE
