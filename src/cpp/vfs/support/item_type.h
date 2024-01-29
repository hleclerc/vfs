#pragma once

#include "../containers/Seq.h"

BEG_VFS_NAMESPACE

TT constexpr auto item_type( CtType<Seq<T>> ) { return CtType<T>(); }

END_VFS_NAMESPACE
