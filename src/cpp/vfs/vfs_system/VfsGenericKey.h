#pragma once

#include "../support/compare.h"

BEG_VFS_NAMESPACE

///
template<class Value>
class VfsGenericKey {
public:
    int         compare( const VfsGenericKey &that ) const { return VFS_NAMESPACE::compare( value, that.value ); }

    Value       value; ///<
};

END_VFS_NAMESPACE
