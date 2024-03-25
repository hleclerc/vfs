#include "VfsTypeTable.h"

BEG_VFS_NAMESPACE

PI32 VfsTypeTable::new_instantiated_type_index() {
    PI32 res = ++nb_instantiated_types;
    if ( res >= array_size )
        TODO;
    return res;
}

END_VFS_NAMESPACE
