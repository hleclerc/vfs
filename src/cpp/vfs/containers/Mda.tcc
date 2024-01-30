#pragma once

#include "Mda.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item,int static_size,int local_size,int alignment,bool allow_heap>
#define UTP Vec<Item,static_size,local_size,alignment,allow_heap>

Mda::Mda() {}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
