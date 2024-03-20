#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

///
class GenericCtValue {
public:
    static auto type_name() { return "GenericCtValue"; }

    Str         cast;
};

// void vfs_object_get_compilation_flags( Vec<Str> &seen, const VirtualCtType &obj ) {
//     push_back_unique( includes, obj.include );
// }

const auto &vfs_object_ct_key( const GenericCtValue &obj ) {
    return obj.cast;
}

Seq<Str> vfs_object_ct_cast( const GenericCtValue &obj, bool deref = true ) {
    return { "auto {ARG} = " + obj.cast + ";" };
}

END_VFS_NAMESPACE
