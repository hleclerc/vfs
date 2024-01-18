#pragma once

#include "../support/string/ctor_of_string_to_string.h"
#include "../support/string/ctor_for.h"

namespace Vfs {

///
class RtString {
public:
    static auto cast_to_string( StrView str ) { auto i = str.find( "CtString<" ); return ctor_of_string_to_string( str.substr( i + 9, str.size() - ( i + 9 + 4 ) ) ); }
    static auto type_name     () { return "RtString"; }

    Str         value;        ///<
};

inline const auto &vfs_object_ct_key( const RtString &obj ) {
    return obj.value;
}

inline Vec<Str> vfs_object_ct_cast( const RtString &obj ) {
    return { "auto {ARG} = CtString<" + ctor_for( obj.value ) + ">();" };
}

} // namespace Vfs
