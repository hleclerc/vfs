#pragma once

#include "Scalar.h"
#include "Array.h"

namespace Vfs {

using Vector = Array<Scalar,ArrayTagList<ArrayTag::ForceNbDimsTo<1>>>;

}
