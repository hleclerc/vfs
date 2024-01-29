#pragma once

#include "Array.h"

BEG_VFS_NAMESPACE

TT using List = Array<T,ArrayTagList<ArrayTag::ForceNbDimsTo<1>>>;

}
