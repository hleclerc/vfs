#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/StaticStorage.h"
#include "../support/type_name.h"
#include "VfsDtTypeAncestor.h"
#include "VfsDtTypeTable.h"

BEG_VFS_NAMESPACE

///
template<class Object,class Content,class... RefAccess>
class VfsDtType : public VfsDtTypeAncestor {
public:
    virtual PI32           get_instantiated_type_index() override;
    virtual void           get_compilation_flags_rec  ( CompilationFlags &res, Vec<Str> &seen ) const override;
    virtual Vec<Str>       final_types                () const override;
    virtual Vec<Str>       final_refs                 () const override;
    virtual Str            cast_type                  () const override;
    virtual DisplayItem*   display                    ( Displayer &ds ) const override;

    PI32                   instantiated_type_index    = 0; ///< used to find the right function in VfsFunc (for small number of virtual arguments)
};

// impl -------------------------------------------------------------------------------------------------------------------
#define DTP template<class Object,class Content,class... RefAccess>
#define UTP VfsDtType<Object,Content,RefAccess...>

DTP PI32 UTP::get_instantiated_type_index() {
    if ( instantiated_type_index == 0 )
        instantiated_type_index = StaticStorage<VfsDtTypeTable,Object>::value.new_instantiated_type_index();
    return instantiated_type_index;
}

DTP void UTP::get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen ) const {
    VFS_NAMESPACE::get_compilation_flags_rec( res, seen, CtType<Content>() );
}

DTP DisplayItem *UTP::display( Displayer &ds ) const {
    return ds.string( type_name<Content>() );
}

DTP Vec<Str> UTP::final_types() const {
    Vec<Str> res;
    ( res.push_back( type_name( CT_DECAYED_TYPE_OF( RefAccess::ref( *(Content *)nullptr ) ) ) ), ... );
    return res;
}

DTP Vec<Str> UTP::final_refs() const {
    Vec<Str> res;
    ( res.push_back( RefAccess::code() ), ... );
    return res;
}

DTP Str UTP::cast_type() const {
    return "VfsDtWrap<" + type_name<Object>() + "," + type_name<Content>() + ">";
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
