#pragma once

#include "KeyWithIndexAndArraySize.h"
#include "../support/StaticStorage.h"
#include "../support/TypeConfig.h"
#include "TdType_.h"

BEG_VFS_NAMESPACE
class CompilationFlags;
END_VFS_NAMESPACE

BEG_VFS_INTERNAL_NAMESPACE

///
template<class FinalWrapper>
class TdKey : public KeyWithIndexAndArraySize {
public:
    TT           TdKey                ( CtType<T> ) { type_index = StaticStorage<TdType_<T>>::value.type_index; }
    /**/         TdKey                () {}

    void         get_compilation_flags( CompilationFlags &cg ) const { TdType<FinalWrapper>::find_by_index( type_index )->get_compilation_flags( cg ); }
    static PI32  array_size           () { return TdType<FinalWrapper>::array_size; }
    Str          cast_type            () const { return TdType<FinalWrapper>::find_by_index( type_index )->cast_type(); }
    PI32         value                () const { return type_index; }

    PI32         type_index;          ///<
};

END_VFS_INTERNAL_NAMESPACE
