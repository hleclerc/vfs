#pragma once

#include "VfsType.h"

BEG_VFS_NAMESPACE

///
template<class Wrapper,class Child>
class alignas( Wrapper::__VfsWrapperAttributes::alig ) VfsObject {
public:
    // using             VfsWrapperAttributes = Wrapper::__VfsWrapperAttributes;

    /**/                 VfsObject               () {  }

    mutable PI32         instantiated_type_index;
    PI32                 global_type_index;
};

END_VFS_NAMESPACE
