#pragma once

#include "../vfs_system/VfsObject.h"
class Int;

BEG_VFS_NAMESPACE

///
template<class IntType>
class VfsInt : public VfsObject<Int,VfsInt<IntType>> {
public:
    static void for_each_template_arg( auto &&f ) { f( CtType<IntType>() ); }
    static auto template_type_name   () { return "VfsInt"; }

    /**/        VfsInt               ( IntType value = 0 ) : value( value ) {}

    IntType     value;
};

END_VFS_NAMESPACE
