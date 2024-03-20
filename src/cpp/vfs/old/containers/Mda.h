#pragma once

#include "Vec.h"

BEG_VFS_NAMESPACE

/// Multi-Dimensional Array
template<class Item,class Size,class Strides,class Parent>
class Mda : public WithDefaultOperators {
public:
    /**/    Mda();


};

END_VFS_NAMESPACE

#include "Mda.tcc"
