#pragma once

#include "TdType.h"

BEG_VFS_INTERNAL_NAMESPACE

#define DTP template<class Wrapper>
#define UTP TdType<Wrapper>

DTP UTP *UTP::last_type = nullptr;
DTP PI32 UTP::array_size = 256;

DTP UTP::TdType() {
    type_index = last_type ? last_type->type_index + 1 : 1;
    if ( type_index >= array_size )
        TODO;

    prev_type = last_type;
    last_type = this;
}

DTP UTP::~TdType() {
}

DTP UTP *UTP::find_by_index( PI32 type_index ) {
    for( TdType *type = last_type; type; type = type->prev_type )
        if ( type->type_index == type_index )
            return type;
    return nullptr;
}

#undef DTP
#undef UTP

END_VFS_INTERNAL_NAMESPACE
