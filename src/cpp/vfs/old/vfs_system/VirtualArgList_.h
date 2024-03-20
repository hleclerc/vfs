#pragma once

#include "VirtualArgList.h"

BEG_VFS_NAMESPACE

///
template<class... Ptrs>
class VirtualArgList_ : public VirtualArgList {
public:
    template<class Ptr> void destroy_ptr( PI index ) {
        auto &ptr = reinterpret_cast<Ptr &>( pointers[ index ] );
        ptr.~Ptr();
    }

    void destroy() {
        PI index = 0;
        ( destroy_ptr<Ptrs>( index++ ), ... );
    }
};

END_VFS_NAMESPACE
