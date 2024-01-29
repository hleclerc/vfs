#pragma once

#include "../containers/Seq.h"

BEG_VFS_NAMESPACE

///
class RtStringList {
public:
    static auto cast_to_string( StrView str ) -> Seq<Str>;

    static auto type_name     () { return "RtStringList"; }

    Seq<Str>    value;        ///<
};

Seq<Str>        vfs_object_ct_cast( const RtStringList &obj );
const Seq<Str> &vfs_object_ct_key ( const RtStringList &obj );

END_VFS_NAMESPACE
