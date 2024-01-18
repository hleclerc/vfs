#pragma once

#include "DisplayContext.h"
#include "DisplayItem.h"

namespace Vfs {

/**
*/
template<class T>
class DisplayItem_Number : public DisplayItem {
public:
    /**/         DisplayItem_Number( const T &val ) : val( val ) {}
    virtual void write_with        ( DisplayContext &dc ) override { *dc << val; }

    T            val;
};

}
