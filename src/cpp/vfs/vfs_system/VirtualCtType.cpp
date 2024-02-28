#include "VirtualCtType.h"

BEG_VFS_NAMESPACE

void VirtualCtType::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/vfs_system/VirtualCtType.h" );
}

Str VirtualCtType::type_name() {
    return "VirtualCtType";
}

bool VirtualCtType::operator<( const VirtualCtType &that ) const {
    if ( name != that.name )
        return name < that.name;
    return cn < that.cn;
}

void VfsArgTrait<VirtualCtType>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Str &cast_ref, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtType &obj ) {
    cf << obj.cn;
    final_types = { "CtType<" + obj.name + ">" };
    final_refs = { final_types[ 0 ] + " {FINAL_NAME};" };
}

END_VFS_NAMESPACE



