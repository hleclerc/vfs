#pragma once

#include "KeyWithIndexAndArraySize.h"
#include "../support/StaticStorage.h"
#include "../support/TypeConfig.h"
#include "VfsTdType_.h"

BEG_VFS_NAMESPACE
class CompilationFlags;

///
template<class FinalWrapper>
class VfsTdKey : public VfsKeyWithIndexAndArraySize {
public:
    TT           VfsTdKey             ( CtType<T> ) { type_index = StaticStorage<VfsTdType_<T>>::value.type_index; }
    /**/         VfsTdKey             () {}

    void         get_compilation_flags( CompilationFlags &cg ) const { VfsTdType<FinalWrapper>::find_by_index( type_index )->get_compilation_flags( cg ); }
    static PI32  array_size           () { return VfsTdType<FinalWrapper>::array_size; }
    Str          cast_type            () const { return VfsTdType<FinalWrapper>::find_by_index( type_index )->cast_type(); }
    PI32         value                () const { return type_index; }

    PI32         type_index;          ///<
};

END_VFS_NAMESPACE
