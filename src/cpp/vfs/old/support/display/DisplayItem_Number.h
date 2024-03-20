#pragma once

#include "DisplayContext.h"
#include "DisplayItem.h"

BEG_VFS_NAMESPACE

/**
*/
template<class T>
class DisplayItem_Number : public DisplayItem {
public:
    /**/         DisplayItem_Number( const T &val ) : val( val ) {}
    virtual void write_with        ( DisplayContext &dc ) override { *dc << val; }

    T            val;
};

END_VFS_NAMESPACE
