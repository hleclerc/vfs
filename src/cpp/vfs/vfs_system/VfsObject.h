#pragma once

#include "VfsType.h"

BEG_VFS_NAMESPACE

///
template<class Wrapper_,class Object>
class alignas( Wrapper_::__VfsWrapperAttributes::alig ) VfsObject {
public:
    using                Wrapper              = Wrapper_;

    /**/                 VfsObject            ();

    mutable PI32         instantiated_type_index;
    PI32                 global_type_index;
};

#define DTP template<class Wrapper,class Object>
#define UTP VfsObject<Wrapper,Object>

DTP UTP::VfsObject() {
    auto &type = StaticStorage<VfsType<Object>>::value;
    instantiated_type_index = type.instantiated_type_index;
    global_type_index = type.global_type_index;
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
