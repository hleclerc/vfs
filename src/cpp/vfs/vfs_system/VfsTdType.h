#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE
class CompilationFlags;

///
template<class Wrapper>
class VfsTdType {
public:
    /**/           VfsTdType               ();
    virtual       ~VfsTdType               ();

    static VfsTdType* find_by_index        ( PI32 type_index );

    virtual void   get_compilation_flags( CompilationFlags &cg ) const = 0;
    virtual Str    cast_type            () const = 0;

    PI32           type_index;          ///<
    VfsTdType*        prev_type;           ///<

    // type table
    static PI32    array_size;          ///<
    static VfsTdType* last_type;           ///<
};

END_VFS_NAMESPACE

#include "VfsTdType.tcc" // IWYU pragma: export
