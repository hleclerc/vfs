#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE
class CompilationFlags;
END_VFS_NAMESPACE

BEG_VFS_INTERNAL_NAMESPACE

///
template<class Wrapper>
class TdType {
public:
    /**/           TdType               ();
    virtual       ~TdType               ();

    static TdType* find_by_index        ( PI32 type_index );

    virtual void   get_compilation_flags( CompilationFlags &cg ) const = 0;
    virtual Str    cast_type            () const = 0;

    PI32           type_index;          ///<
    TdType*        prev_type;           ///<

    // type table
    static PI32    array_size;          ///<
    static TdType* last_type;           ///<
};

END_VFS_INTERNAL_NAMESPACE

#include "TdType.tcc" // IWYU pragma: export
