#pragma once

#include "../support/operators/ceil.h" // IWYU pragma: export
#include "../support/call_new.h" // IWYU pragma: export
#include "TdKey.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class FinalWrapper,int size,int alig>
class alignas( alig ) TdWrapper {
public:
    static constexpr PI ds          = ceil( sizeof( TdKey<FinalWrapper> ), alig ) + size - sizeof( TdKey<FinalWrapper> );

    TdKey<FinalWrapper> type_info;  ///<
    char                data[ ds ]; ///<
};

template<class FinalWrapper,int size,int alig>
auto wrapper_keys( const TdWrapper<FinalWrapper,size,alig> &wrapper ) { return tie( wrapper.type_info ); }

END_VFS_INTERNAL_NAMESPACE
