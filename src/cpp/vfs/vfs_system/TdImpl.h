#pragma once

#include "../support/StaticStorage.h"
#include "TdType_.h"
#include "TdKey.h"

BEG_VFS_INTERNAL_NAMESPACE

/**
 * Proxy/pointer to symbolic or real reference
*/
template<class Wrapper_,class Impl>
class TdImpl {
public:
    using               FinalWrapper = Wrapper_;

    /**/                TdImpl       () { StaticStorage<TdType_<Impl>>::value.init_impl( static_cast<Impl &>( *this ) ); }

    TdKey<FinalWrapper> key;         ///<
};

END_VFS_INTERNAL_NAMESPACE
