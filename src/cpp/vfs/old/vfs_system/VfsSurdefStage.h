#pragma once

#include "../containers/Vec.h"

BEG_VFS_NAMESPACE

///
struct VfsSurdefStage {
    enum class  Result { invalid, needs_a_second_pass, valid };

    Vec<double> pertinence;
    Result      result;
};

END_VFS_NAMESPACE
