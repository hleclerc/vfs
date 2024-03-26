#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/StaticStorage.h"
#include "../support/type_name.h"
#include "VfsTypeAncestor.h"
#include "VfsTypeTable.h"

BEG_VFS_NAMESPACE

///
template<class Object>
class VfsType : public VfsTypeAncestor {
public:
    virtual PI32           get_instantiated_type_index() override;
    virtual void           get_compilation_flags_rec  ( CompilationFlags &res, Vec<Str> &seen ) const override;
    virtual Str            cast_type                  () const override;
    virtual DisplayItem*   display                    ( Displayer &ds ) const override;

    PI32                   instantiated_type_index    = 0; ///< used to find the right function in VfsFunc (for small number of virtual arguments)
};

// impl -------------------------------------------------------------------------------------------------------------------
#define DTP template<class Object>
#define UTP VfsType<Object>

DTP PI32 UTP::get_instantiated_type_index() {
    if ( instantiated_type_index == 0 )
        instantiated_type_index = StaticStorage<VfsTypeTable,Object>::value.new_instantiated_type_index();
    return instantiated_type_index;
}

DTP void UTP::get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen ) const {
    VFS_NAMESPACE::get_compilation_flags_rec( res, seen, CtType<Object>() );
}

DTP DisplayItem *UTP::display( Displayer &ds ) const {
    return ds.string( type_name<Object>() );
}

DTP Str UTP::cast_type() const {
    return type_name<Object>();
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
