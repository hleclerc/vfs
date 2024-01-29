#pragma once

#include "../containers/Vec.h"

BEG_VFS_NAMESPACE

///
class RtStringList {
public:
    static auto cast_to_string( StrView str ) -> Vec<Str>;

    static auto type_name     () { return "RtStringList"; }

    Vec<Str>    value;        ///<
};

Vec<Str>        vfs_object_ct_cast( const RtStringList &obj );
const Vec<Str> &vfs_object_ct_key ( const RtStringList &obj );

END_VFS_NAMESPACE
