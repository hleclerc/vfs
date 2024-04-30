#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "VfsTdType.h"

BEG_VFS_NAMESPACE

///
template<class Impl>
class VfsTdType_ : public VfsTdType<typename Impl::Wrapper> {
public:
    virtual void get_compilation_flags( CompilationFlags &cg ) const override { Vec<Str> seen; get_compilation_flags_rec( cg, seen, CtType<Impl>() ); }
    virtual Str  cast_type            () const override { return type_name<Impl>(); }
};

END_VFS_NAMESPACE
