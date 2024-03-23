#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

///
template<class Wrapper,class Child>
class alignas( Wrapper::__VfsWrapperAttributes::alig ) VfsObject {
public:
    using                VfsWrapperAttributes = Wrapper::__VfsWrapperAttributes;



    VfsWrapperAttributes attributes;
};

END_VFS_NAMESPACE
