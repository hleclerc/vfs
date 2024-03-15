#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

template<class ArrayPtr,class Item,class Indices>
class SelectArray {
public:
    static auto  get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/SelectArray.h" ); }
    static void  for_each_template_arg( auto &&f ) { f( CtType<ArrayPtr>() ); f( CtType<Item>() ); f( CtType<Indices>() ); }
    static auto  template_type_name   () { return "SelectArray"; }

    TT           operator T           () const { return value(); }
    DisplayItem* display              ( auto &ds ) const { return VFS_NAMESPACE::display( ds, value() ); }
    auto         value                () const { return std::apply( [&]( const auto &...indices ) { return array_ptr->get_item( indices... ); }, indices ); }

    void         operator=            ( auto &&value ) { std::apply( [&]( const auto &...indices ) { return array_ptr->set_item( FORWARD( value ), indices... ); }, indices ); }

    ArrayPtr     array_ptr;
    Indices      indices;
};

template<class Item>
Item make_SelectArray( CtType<Item>, auto *array_ptr, auto&&...indices ) {
    return { FromValue(), SelectArray<DECAYED_TYPE_OF( array_ptr ),DECAYED_TYPE_OF( indices )...>{ FORWARD( array_ptr ), { FORWARD( indices )... } } };
}

END_VFS_NAMESPACE
