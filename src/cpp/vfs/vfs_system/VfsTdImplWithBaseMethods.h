#pragma once

#include "VfsTdImpl.h"

BEG_VFS_NAMESPACE

///
template<class Wrapper,class Impl,class Data>
struct VfsTdImplWithBaseMethods : VfsTdImpl<Wrapper,Impl,Data> {
    /**/         VfsTdImplWithBaseMethods( auto &&...args );

    explicit     operator Data           () const { return this->wrapped_value(); }
    auto         reassign_pmt            ( auto &&cb, auto &&that ); ///< pmt = potentially modifiable type
    auto         self_op_pmt             ( auto &&cb, auto &&functor, auto &&that ); ///< pmt = potentially modifiable type

    DisplayItem* display                 ( auto &ds ) const;
};


END_VFS_NAMESPACE

#include "VfsTdImplWithBaseMethods.tcc" // IWYU pragma: export
