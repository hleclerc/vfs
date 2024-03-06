#include "string_replace.h"

BEG_VFS_NAMESPACE

Str string_replace( StrView str, StrView ori, StrView dst ) {
    Str res;
    for( StrView::size_type old_index = 0;; ) {
        auto new_index = str.find( ori, old_index );
        if ( new_index == StrView::npos ) {
            res += str.substr( old_index );
            return res;
        }

        res += str.substr( old_index, new_index - old_index );
        res += dst;

        old_index = new_index + ori.size();
    }
}

END_VFS_NAMESPACE
