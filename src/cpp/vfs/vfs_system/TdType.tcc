#include "TdType.h"

BEG_VFS_INTERNAL_NAMESPACE

TdType *TdType::last_unregistered_type = nullptr;

TdType::TdType() {
    prev_unregistered_type = last_unregistered_type;
    last_unregistered_type = this;
}

TdType::~TdType() {
}

END_VFS_INTERNAL_NAMESPACE
