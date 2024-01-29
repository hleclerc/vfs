#include "DisplayItem_String.h"
#include "DisplayContext.h"

BEG_VFS_NAMESPACE

DisplayItem_String::DisplayItem_String( std::string_view str ) : str( str ) {
}

void DisplayItem_String::write_with( DisplayContext &dc ) {
    if ( dc.need_delimiters )
        *dc << '"';
    *dc << str;
    if ( dc.need_delimiters )
        *dc << '"';
}

END_VFS_NAMESPACE
