#pragma once

#include "VfsTdType.h"

BEG_VFS_NAMESPACE

#define DTP template<class Wrapper>
#define UTP VfsTdType<Wrapper>

DTP UTP *UTP::last_type = nullptr;
DTP PI32 UTP::array_size = 256;

DTP UTP::VfsTdType() {
    type_index = last_type ? last_type->type_index + 1 : 1;
    if ( type_index >= array_size )
        TODO;

    prev_type = last_type;
    last_type = this;
}

DTP UTP::~VfsTdType() {
}

DTP UTP *UTP::find_by_index( PI32 type_index ) {
    for( VfsTdType *type = last_type; type; type = type->prev_type )
        if ( type->type_index == type_index )
            return type;
    return nullptr;
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
