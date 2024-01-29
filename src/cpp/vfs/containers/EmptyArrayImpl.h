#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

struct EmptyArrayImpl {
    static void     get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/EmptyArrayImpl.h" ); }
    void            for_each_item        ( const auto & ) const {}
    static auto     type_name            () { return "EmptyArrayImpl"; }
    DisplayItem*    display              ( Displayer &ds ) const { return ds.array( {} ); }
    static CtInt<0> nb_dims              () { return {}; }
    static CtInt<0> size                 () { return {}; }
};

inline auto ct_value( EmptyArrayImpl v ) { return v; }

END_VFS_NAMESPACE
