#pragma once

#include "../support/StaticStorage.h"
#include "../support/TypeConfig.h"
#include "TdType_.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class FinalWrapper>
class TdKey {
public:
    TT    TdKey       ( CtType<T> ) { type_index = StaticStorage<TdType_<T>>::value.type_index; }
    /**/  TdKey       () {}

    PI32  array_size  () const { return TdType<FinalWrapper>::array_size; }
    Str   cast_type   () const { return TdType<FinalWrapper>::find_by_index( type_index )->cast_type(); }
    PI32  value       () const { return type_index; }

    PI32  type_index; ///<
};

END_VFS_INTERNAL_NAMESPACE
