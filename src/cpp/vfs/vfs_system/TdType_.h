#pragma once

#include "../support/StaticStorage.h"
#include "TdTypeTable.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class Impl>
class TdType_ : public TdType {
public:
    using        FinalWrapper = Impl::FinalWrapper;

    /**/         TdType_      () { type_index = type_table().new_type_index(); }

    virtual void register_type() { return StaticStorage<TdTypeTable,FinalWrapper>::value.register_type( this ); }
    virtual Str  cast_type    () const { return type_name<Impl>(); }

    static auto& type_table   () { return StaticStorage<TdTypeTable,FinalWrapper>::value; }

    PI32         type_index;  ///<
};

END_VFS_INTERNAL_NAMESPACE
