#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

/// nb_dim = -1 => undefined
template<int nb_dims_>
struct EmptyArrayImpl {
    static void           get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/EmptyArrayImpl.h" ); }
    static void           for_each_template_arg( auto &&f ) { f( CtInt<nb_dims_>() ); }
    static auto           template_type_name   () { return "EmptyArrayImpl"; }
    void                  for_each_item        ( const auto & ) const {}
    DisplayItem*          display              ( Displayer &ds ) const { return ds.array( {} ); }
    constexpr static auto nb_dims              () { return CtInt<nb_dims_>(); }
    constexpr static auto size                 () { return CtInt<0>(); }
};

Ti inline auto ct_value( EmptyArrayImpl<i> v ) { return v; }

END_VFS_NAMESPACE
