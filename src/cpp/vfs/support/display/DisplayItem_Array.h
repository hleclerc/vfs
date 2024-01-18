#pragma once

#include "DisplayItem.h"
#include <vector>

namespace Vfs {

/**
*/
class DisplayItem_Array : public DisplayItem {
public:
    using        Items       = std::vector<DisplayItem *>;

    virtual auto column_sizes( DisplayContext &dc ) -> std::vector<int> override;
    virtual void write_with  ( DisplayContext &dc ) override;
    virtual int  max_dim     () const override;
    virtual bool use_cr      ( DisplayContext &dc ) const override;

    bool         homogeneous = true;
    Items        items;
};

}
