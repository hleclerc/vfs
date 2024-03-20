#pragma once

#include "DisplayItem.h"
#include <vector>

BEG_VFS_NAMESPACE

/**
*/
class DisplayItem_Object : public DisplayItem {
public:
    struct                 Attribute   { std::string name; DisplayItem *item; };

    virtual auto           column_sizes( DisplayContext &dc ) -> std::vector<int> override;
    virtual auto           column_keys ( DisplayContext &dc ) -> std::vector<std::string> override;
    virtual void           write_with  ( DisplayContext &dc ) override;
    virtual int            max_dim     () const override;
    virtual bool           use_cr      ( DisplayContext &dc ) const override;

    std::string            name;
    std::vector<Attribute> attributes; ///<
};

END_VFS_NAMESPACE
