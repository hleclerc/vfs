#pragma once

#include "Scalar.h"
#include "Array.h"

namespace Vfs {

using Point = Array<Scalar,ArrayTagList<ArrayTag::ForceNbDimsTo<1>,ArrayTag::WantCtSizeInAllDims>>;

}
