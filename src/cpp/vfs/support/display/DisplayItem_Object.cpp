#include "DisplayItem_Object.h"
#include "DisplayContext.h"

namespace Vfs {

std::vector<int> DisplayItem_Object::column_sizes( DisplayContext &dc ) {
    std::vector<int> res;
    for( std::size_t i = 0; i < attributes.size(); ++i ) {
        auto s = attributes[ i ].item->size_with( dc );
        if ( ! dc.need_type_info )
            s += attributes[ i ].name.size() + 1 + dc.parameters->use_spaces_for_readability;
        res.push_back( s );
    }
    return res;
}

std::vector<std::string> DisplayItem_Object::column_keys( DisplayContext &dc ) {
    std::vector<std::string> res;
    for( const auto &attr : attributes )
        res.push_back( attr.name );
    return res;
}

void DisplayItem_Object::write_with( DisplayContext &dc ) {
    bool single_line = ! use_cr( dc );

    if ( dc.need_type_info )
        *dc << name;

    if ( name.size() && single_line ) {
        *dc << "{";
        if ( dc.parameters->use_spaces_for_readability && attributes.size() )
            *dc << ' ';
    }

    for( std::size_t i = 0; i < attributes.size(); ++i ) {
        if ( i ) {
            *dc << dc.parameters->inline_list_delimiter;
            if ( dc.parameters->use_spaces_for_readability )
                *dc << ' ';
        }

        if ( dc.need_type_info ) {
            *dc << attributes[ i ].name << ':';
            if ( dc.parameters->use_spaces_for_readability )
                *dc << ' ';
        }


        attributes[ i ].item->write_with( dc );
    }

    if ( name.size() && single_line ) {
        if ( dc.parameters->use_spaces_for_readability && attributes.size() )
            *dc << ' ';
        *dc << "}";
    }
}

int DisplayItem_Object::max_dim() const {
    int res = 0;
    for( const auto &attr : attributes )
        res = std::max( res, attr.item->max_dim() );
    return res + 1;
}

bool DisplayItem_Object::use_cr( DisplayContext &dc ) const {
    return false;
}

}
