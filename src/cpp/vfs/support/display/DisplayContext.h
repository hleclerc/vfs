#pragma once

#include "DisplayParameters.h"
#include <ostream>
#include <vector>

namespace Vfs {

///
struct DisplayContext {
    using          Parameters         = const DisplayParameters *;
    using          Columns            = std::vector<int>;

    void           write_column_suffix( int num_column );
    void           write_beg_line     ();
    std::ostream&  operator*          (); ///< just a shortcut to get *os

    void           inc_tabs           ();
    void           dec_tabs           ();

    bool           has_its_own_line   = true;
    bool           need_delimiters    = false;
    bool           need_type_info     = true;
    Columns*       columns_curr       = nullptr;
    Columns*       columns_max        = nullptr;
    Parameters     parameters         = nullptr;
    std::string    beg_line           = {};
    std::ostream*  os                 = nullptr;
};

}
