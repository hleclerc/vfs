#pragma once

#include "VfsTdKey.h"

BEG_VFS_NAMESPACE

/**
 * Proxy/pointer to symbolic or real reference
*/
template<class Wrapper_,class Impl>
class VfsTdImpl {
public:
    using                  FinalWrapper = Wrapper_;

    /**/                   VfsTdImpl    () : key( CtType<Impl>() ) {}

    VfsTdKey<FinalWrapper> key;         ///<
};

END_VFS_NAMESPACE
