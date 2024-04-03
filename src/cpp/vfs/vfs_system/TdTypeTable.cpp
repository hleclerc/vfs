#include "TdTypeTable.h"

BEG_VFS_INTERNAL_NAMESPACE

PI32 TdTypeTable::new_type_index() {
    PI32 res = ++last_type_index;
    if ( res >= array_size )
        TODO;
    return res;
}

void TdTypeTable::register_type( TdType *type ) { types[ type->type_index ] = type; }


END_VFS_INTERNAL_NAMESPACE
