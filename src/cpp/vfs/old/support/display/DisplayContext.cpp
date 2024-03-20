#include "DisplayContext.h"

BEG_VFS_NAMESPACE

void DisplayContext::write_column_suffix( int num_column ) {
    if ( columns_curr && columns_max )
        for( int i = 0; i < columns_max->at( num_column ) - columns_curr->at( num_column ); ++i )
            *os << ' ';
}

void DisplayContext::write_beg_line() {
    if ( has_its_own_line ) {
        has_its_own_line = false;
        return;
    }
    *os << "\n" << beg_line;
}

void DisplayContext::inc_tabs() {
    if ( parameters->tab_size < 0 )
        beg_line += '\t';
    else
        for( int i = 0; i < parameters->tab_size; ++i )
            beg_line += ' ';
}

void DisplayContext::dec_tabs() {
    beg_line.resize( beg_line.size() - ( parameters->tab_size < 0 ? 1 : parameters->tab_size ) );
}

std::ostream &DisplayContext::operator*() {
    return *os;
}

END_VFS_NAMESPACE
