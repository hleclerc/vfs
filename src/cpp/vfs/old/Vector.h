#pragma once

#include "Scalar.h"
#include "Array.h"

BEG_VFS_NAMESPACE

using Vector = Array<Scalar,ArrayTag::ForceNbDimsTo<1>>;

END_VFS_NAMESPACE
