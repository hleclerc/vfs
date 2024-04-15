#pragma once

#include "../support/CompilationFlags.h"
#include "VfsTdType.h"

BEG_VFS_NAMESPACE

///
template<class Impl>
class VfsTdType_ : public VfsTdType<typename Impl::FinalWrapper> {
public:
    virtual void get_compilation_flags( CompilationFlags &cg ) const override { VFS_NAMESPACE::get_compilation_flags( cg, CtType<Impl>() ); }
    virtual Str  cast_type            () const override { return type_name<Impl>(); }
};

END_VFS_NAMESPACE
