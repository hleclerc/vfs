#pragma once

// #include "VfsObjectTypeFor.h" // IWYU pragma: export
#include "../support/StaticStorage.h"
#include "VfsType.h"

BEG_VFS_NAMESPACE

/**
 * Proxy/pointer to symbolic or real reference
*/
template<class Wrapper,class ChildImpl>
class VfsImpl {
public:
    /**/         VfsImpl                  () { StaticStorage<VfsType,Wrapper,ChildImpl>::value }

    mutable PI32 instantiated_type_index; ///< index in _used_ pointer types (i.e. that have been used at least once in a function call). Wrapper specific (there's a table for Int, another one the Scalar, etc...).
    PI32         global_type_index;       ///<  index of ptr type, amongst all the existing pointer types. NOT wrapper specific.
};

END_VFS_NAMESPACE
