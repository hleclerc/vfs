#pragma once

#include "TensorOrder.h"

BEG_VFS_NAMESPACE

template<class T>
concept IsScalar = TensorOrder<T>::value == 0;

END_VFS_NAMESPACE
