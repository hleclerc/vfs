#include "DisplayItem_String.h"
#include "DisplayContext.h"

namespace Vfs {

DisplayItem_String::DisplayItem_String( std::string_view str ) : str( str ) {
}

void DisplayItem_String::write_with( DisplayContext &dc ) {
    if ( dc.need_delimiters )
        *dc << '"';
    *dc << str;
    if ( dc.need_delimiters )
        *dc << '"';
}

}
