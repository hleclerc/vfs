#pragma once

#include "support/string/ctor_for.h" // IWYU pragma: export
#include "vfs_system/VfsWrapper.h" // IWYU pragma: export
//#include "VfsTdKey.h"

BEG_VFS_NAMESPACE

///
template<class Key,CtStringValue ct_name>
class CtValueMaker : public VfsWrapper {
public:
    static void  get_compilation_flags        ( auto &cn ) { cn.add_inc_file( "vfs/make_ct_value.h" ); }
    static void  for_each_template_arg        ( auto &&f ) { f( CtType<Key>() ); f( CtString<ct_name>() ); }
    static auto  template_type_name           () { return "CtValueMaker"; }

    void         wrapper_get_compilation_flags( CompilationFlags &cg ) const { /*get_compilation_flags_rec( cg, key );*/ }
    Str          wrapper_cast_type            () const { return ct_name.to_string() + "<" + ctor_for( key ) + ">"; }
    auto         wrapper_keys                 () const { return tie( key ); }

    Key          key;                         ///<
};

template<int i> constexpr auto make_ct_value( CtInt<i> ) { return CtInt<i>(); }

inline auto make_ct_value( SI32 value ) { return CtValueMaker<SI32,"CtInt">{ .key = value }; }

END_VFS_NAMESPACE
