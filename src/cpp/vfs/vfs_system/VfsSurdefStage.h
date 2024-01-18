#pragma once

#include "../support/TypeConfig.h"

namespace Vfs {

///
struct VfsSurdefStage {
    enum class  Result { invalid, needs_a_second_pass, valid };

    Vec<double> pertinence;
    Result      result;
};

}
