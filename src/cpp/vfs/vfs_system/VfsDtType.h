#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/StaticStorage.h"
#include "../support/type_name.h"
#include "VfsDtTypeAncestor.h"
#include "VfsDtTypeTable.h"

BEG_VFS_NAMESPACE

///
template<class Object,class Content,int nb_indirections_>
class VfsDtType : public VfsDtTypeAncestor {
public:
    virtual PI32           get_instantiated_type_index() override;
    virtual void           get_compilation_flags_rec  ( CompilationFlags &res, Vec<Str> &seen ) const override;
    virtual int            nb_indirections            () const override;
    virtual Vec<Str>       final_types                () const override;
    virtual Vec<Str>       final_refs                 () const override;
    virtual Str            cast_type                  () const override;
    virtual DisplayItem*   display                    ( Displayer &ds ) const override;

    PI32                   instantiated_type_index    = 0; ///< used to find the right function in VfsFunc (for small number of virtual arguments)
};

// impl -------------------------------------------------------------------------------------------------------------------
#define DTP template<class Object,class Content,int nb_indirections_>
#define UTP VfsDtType<Object,Content,nb_indirections_>

DTP PI32 UTP::get_instantiated_type_index() {
    if ( instantiated_type_index == 0 )
        instantiated_type_index = StaticStorage<VfsDtTypeTable,Object>::value.new_instantiated_type_index();
    return instantiated_type_index;
}

DTP void UTP::get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen ) const {
    VFS_NAMESPACE::get_compilation_flags_rec( res, seen, CtType<Content>() );
}

DTP int UTP::nb_indirections() const {
    return nb_indirections_;
}

DTP DisplayItem *UTP::display( Displayer &ds ) const {
    return ds.string( type_name<Content>() );
}

DTP Vec<Str> UTP::final_types() const {
    return { type_name<Content>() };
}

DTP Vec<Str> UTP::final_refs() const {
    return { "{ARG_CSTNESS}auto {ARG_REFNESS}{FINAL_NAME} = {CAST_NAME}.data.content;" };
}

DTP Str UTP::cast_type() const {
    return "VfsDtWrap_<" + type_name<Object>() + "," + type_name<Content>() + ">";
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
