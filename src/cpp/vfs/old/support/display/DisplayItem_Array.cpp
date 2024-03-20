#include "DisplayItem_Array.h"
#include "DisplayContext.h"
#include "../RaiiSave.h"
 
BEG_VFS_NAMESPACE

std::vector<int> DisplayItem_Array::column_sizes( DisplayContext &dc ) {
    std::vector<int> res;
    for( std::size_t i = 0; i < items.size(); ++i )
        res.push_back( items[ i ]->size_with( dc ) );
    return res;
}

void DisplayItem_Array::write_with( DisplayContext &dc ) {
    // in different lines
    if ( use_cr( dc ) ) {
        // column sizes
        std::vector<std::vector<int>> columns;
        for( std::size_t i = 0; i < items.size(); ++i )
            columns.push_back( items[ i ]->column_sizes( dc ) );
        auto mc = max_columns( columns );

        // column keys
        if ( homogeneous && items.size() ) {
            std::vector<std::string> keys = items[ 0 ]->column_keys( dc );
            if ( keys.size() ) {
                dc.write_beg_line();
                int spd = dc.parameters->inline_list_delimiter.size() + dc.parameters->use_spaces_for_readability;
                for( std::size_t i = 0; i < std::min( keys.size(), mc.size() ); ++i ) {
                    mc[ i ] = std::max( mc[ i ], int( keys[ i ].size() ) );
                    if ( dc.parameters->escape_sequence_for_keys.size() )
                        *dc << "\033[" << dc.parameters->escape_sequence_for_keys << "m"; //
                    *dc << keys[ i ];
                    if ( dc.parameters->escape_sequence_for_keys.size() )
                        *dc << "\033[0m";
                    for( int k = 0; k < mc[ i ] + spd - keys[ i ].size(); ++k )
                        *dc << ' ';
                }
                for( std::size_t i = keys.size(); i < mc.size(); ++i )
                   for( int k = 0; k < mc[ i ] + spd; ++k )
                        *dc << ' ';
            }
        }

        // write items
        auto _0 = raii_save( dc.need_delimiters, ! homogeneous );
        auto _1 = raii_save( dc.need_type_info, ! homogeneous );
        auto _2 = raii_save( dc.columns_max, &mc );
        for( std::size_t i = 0; i < items.size(); ++i ) {
            auto _0 = raii_save( dc.columns_curr, &columns[ i ] );
            dc.write_beg_line();
            dc.inc_tabs();
            items[ i ]->write_with( dc );
            dc.dec_tabs();
        }
        return;
    }

    // in a single line
    if ( dc.need_delimiters ) {
        *dc << "[";
        if ( dc.parameters->use_spaces_for_readability && items.size() )
            *dc << " ";
    }
    for( std::size_t i = 0; i < items.size(); ++i ) {
        if ( i ) {
            *dc << dc.parameters->inline_list_delimiter;
            if ( dc.parameters->use_spaces_for_readability )
                *dc << " ";
        }

        auto _0 = raii_save( dc.need_delimiters, true );
        auto _1 = raii_save( dc.need_type_info, ! homogeneous );
        items[ i ]->write_with( dc );

        dc.write_column_suffix( i );
    }
    if ( dc.need_delimiters ) {
        if ( dc.parameters->use_spaces_for_readability && items.size() )
            *dc << " ";
        *dc << "]";
    }
}

int DisplayItem_Array::max_dim() const {
    int res = 0;
    for( const DisplayItem *item : items )
        res = std::max( res, item->max_dim() );
    return res + 1;
}

bool DisplayItem_Array::use_cr( DisplayContext &dc ) const {
    if ( dc.parameters->single_line )
        return false;
    if ( max_dim() > 1 )
        return true;
    for( const DisplayItem *item : items )
        if ( item->use_cr( dc ) )
            return true;
    return false;
}

END_VFS_NAMESPACE
