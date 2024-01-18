#pragma once

#include "../support/TypeConfig.h"

namespace Vfs {

///
struct VfsTdTypeTable {
    PI32 new_instantiated_type_index();

    PI32 nb_instantiated_types      = 0;   ///<
    PI32 array_size                 = 256; ///< func table init their arrays with this number. If nb_inst_types > max_inst_types, func table must be resized
};

}
