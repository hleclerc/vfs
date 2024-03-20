#include "VirtualCtType.h"

BEG_VFS_NAMESPACE

void VirtualCtType::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/vfs_system/VirtualCtType.h" );
}

Str VirtualCtType::type_name() {
    return "VirtualCtType";
}

int VirtualCtType::compare( const VirtualCtType &that ) const {
    return compare_chain( name, that.name, cn, that.cn );
}

void VfsArgTrait<VirtualCtType>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtType &obj ) {
    cf << obj.cn;

    Str type = "CtType<" + obj.name + ">";
    final_refs = { type + "()" };
    final_types = { type };
}

END_VFS_NAMESPACE



