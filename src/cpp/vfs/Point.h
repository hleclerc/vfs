#pragma once

#include "Scalar.h"
#include "Array.h"

BEG_VFS_NAMESPACE

using Point = Array<Scalar,ArrayTag::ForceNbDimsTo<1>,ArrayTag::WantCtSizeInAllDims>;

END_VFS_NAMESPACE
