#include "DisplayItem_Object.h"
#include "DisplayItem_String.h"
#include "DisplayItem_Array.h"
#include "Displayer.h"

BEG_VFS_NAMESPACE

DisplayItem *Displayer::string( std::string_view str ) {
    return pool.create<DisplayItem_String>( str );
}

DisplayItem *Displayer::object( std::string_view name, const std::vector<std::pair<std::string,DisplayItem *>> &items ) {
    DisplayItem_Object *res = pool.create<DisplayItem_Object>();
    res->name = name;
    for( const auto &item : items )
        res->attributes.push_back( DisplayItem_Object::Attribute{ item.first, item.second } );
    return res;
}

DisplayItem *Displayer::array( const std::vector<DisplayItem *> &items ) {
    DisplayItem_Array *res = pool.create<DisplayItem_Array>();
    res->items = items;
    return res;
}

END_VFS_NAMESPACE
