#pragma once

#include "../support/StaticStorage.h"
#include "../support/TypeConfig.h"
#include "TdTypeTable.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class FinalWrapper>
class TdKey {
public:
    auto& type_table  () const { return StaticStorage<TdTypeTable,FinalWrapper>::value; }
    PI32  array_size  () const { return type_table().reservation; }
    Str   cast_type   () const { return type_table().types[ type_index ]->cast_type(); }
    PI32  value       () const { return type_index; }

    PI32  type_index; ///<
};

END_VFS_INTERNAL_NAMESPACE
