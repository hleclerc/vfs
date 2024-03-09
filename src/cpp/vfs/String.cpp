#include "String.h"

BEG_VFS_NAMESPACE

Int String::size() const {
    return VFS_CALL_METHOD( size, Int, *this );
}

String::operator Str() const {
    return VFS_CALL( Str, CtStringList<>, Str, *this );
}

END_VFS_NAMESPACE
