#pragma once

#include "CtStringValue.h"

namespace Vfs {

///
template<CtStringValue str>
struct CtString {
    static constexpr CtStringValue value = str;
};

}
