#pragma once

#include "../BumpPointerPool.h"
#include "DisplayItem_Number.h"
#include <string_view>

BEG_VFS_NAMESPACE

/// a class to create DisplayItem
class Displayer {
public:
    static auto     include_path() { return "vfs/support/display/Displayer.h"; }
    static auto     type_name   () { return "VFS_NAMESPACE::Displayer"; }

    template        <class T>
    DisplayItem*    number      ( const T &val ) { return pool.create<DisplayItem_Number<T>>( val ); }
    DisplayItem*    object      ( std::string_view name, const std::vector<std::pair<std::string,DisplayItem *>> &items );
    DisplayItem*    string      ( std::string_view str );
    DisplayItem*    array       ( const std::vector<DisplayItem *> &items );

private:
    BumpPointerPool pool;
};

END_VFS_NAMESPACE
