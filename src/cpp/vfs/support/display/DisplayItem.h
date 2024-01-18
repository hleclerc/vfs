#pragma once

#include "DisplayParameters.h"
#include <ostream>
#include <vector>

namespace Vfs {
class DisplayContext;

/**
*/
class DisplayItem {
public:
    virtual     ~DisplayItem ();

    int          size_with   ( DisplayContext &dc );
    void         write_to    ( std::ostream &os, const DisplayParameters &dp = {} );

    virtual auto column_sizes( DisplayContext &dc ) -> std::vector<int>;
    virtual auto column_keys ( DisplayContext &dc ) -> std::vector<std::string>;
    virtual void write_with  ( DisplayContext &dc ) = 0;
    virtual int  max_dim     () const;
    virtual bool use_cr      ( DisplayContext &dc ) const;

    // helpers
    static auto  max_columns ( const std::vector<std::vector<int>> columns ) -> std::vector<int>;

    // type info
    static auto  include_for () { return "vfs/support/display/DisplayItem.h"; }
    static auto  type_name   () { return "DisplayItem"; }
};

}
