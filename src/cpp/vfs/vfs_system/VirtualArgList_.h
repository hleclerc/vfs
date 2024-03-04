#pragma once

#include "VirtualArgList.h"

BEG_VFS_NAMESPACE

///
template<class... Ptrs>
class VirtualArgList_ : public VirtualArgList {
public:
    TT void destroy_ptr( PI index ) {
        auto *ptr = reinterpret_cast<T *>( pointers[ index ] );
        ptr->~T();
    }

    void destroy() {
        PI index = 0;
        ( destroy_ptr<Ptrs>( index++ ), ... );
    }
};

END_VFS_NAMESPACE
