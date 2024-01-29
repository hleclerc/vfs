#pragma once

#include "DisplayItem.h"
#include <string_view>
#include <string>

BEG_VFS_NAMESPACE

/**
*/
class DisplayItem_String : public DisplayItem {
public:
    /**/         DisplayItem_String( std::string_view str );
    virtual void write_with        ( DisplayContext &dc ) override;

    std::string  str;
};

END_VFS_NAMESPACE
