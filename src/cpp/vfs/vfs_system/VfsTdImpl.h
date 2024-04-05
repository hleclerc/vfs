#pragma once

#include "TdKey.h"

BEG_VFS_INTERNAL_NAMESPACE

/**
 * Proxy/pointer to symbolic or real reference
*/
template<class Wrapper_,class Impl>
class TdImpl {
public:
    using               FinalWrapper = Wrapper_;

    /**/                TdImpl       () : key( CtType<Impl>() ) {}

    TdKey<FinalWrapper> key;         ///<
};

END_VFS_INTERNAL_NAMESPACE
