#pragma once

#include "../support/TypeConfig.h"

namespace Vfs {

///
class GenericCtValue {
public:
    static auto type_name() { return "GenericCtValue"; }

    Str         cast;
};

// void vfs_object_get_compilation_flags( Vec<Str> &seen, const RtType &obj ) {
//     push_back_unique( includes, obj.include );
// }

const auto &vfs_object_ct_key( const GenericCtValue &obj ) {
    return obj.cast;
}

Vec<Str> vfs_object_ct_cast( const GenericCtValue &obj ) {
    return { "auto {ARG} = " + obj.cast + ";" };
}

} // namespace Vfs
