#include "DisplayContext.h"
#include "DisplayItem.h"
#include <sstream>

BEG_VFS_NAMESPACE

DisplayItem::~DisplayItem() {
}

int DisplayItem::size_with( DisplayContext &dc ) {
    // TODO OPTIMIZATION: use a specific ostream to store the size
    DisplayContext ndc = dc;
    std::ostringstream ss;
    ndc.os = &ss;

    write_with( ndc );

    return ss.str().size();
}

void DisplayItem::write_to( std::ostream &os, const DisplayParameters &dp ) {
    DisplayContext dc;
    dc.parameters = &dp;
    dc.os = &os;

    write_with( dc );
}

std::vector<int> DisplayItem::column_sizes( DisplayContext &dc ) {
    return { size_with( dc ) };
}

std::vector<std::string> DisplayItem::column_keys( DisplayContext &dc ) {
    return {};
}

int DisplayItem::max_dim() const {
    return 0;
}

bool DisplayItem::use_cr( DisplayContext &dc ) const {
    return false;
}

std::vector<int> DisplayItem::max_columns( const std::vector<std::vector<int>> columns ) {
    std::vector<int> res;
    for( const std::vector<int> &column : columns )
        if ( column.size() > res.size() )
            res.resize( column.size(), 0 );
    for( const std::vector<int> &column : columns )
        for( std::size_t i = 0; i < column.size(); ++i )
            res[ i ] = std::max( res[ i ], column[ i ] );
    return res;
}

END_VFS_NAMESPACE
