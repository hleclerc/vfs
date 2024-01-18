#include "VfsTdTypeTable.h"

namespace Vfs {

PI32 VfsTdTypeTable::new_instantiated_type_index() {
    PI32 res = ++nb_instantiated_types;
    if ( res >= array_size )
        TODO;
    return res;
}

}
