#include "../support/string/ctor_of_string_to_string.h"
#include "../support/CompilationFlags.h"
#include "../support/string/ctor_for.h"
#include "VirtualCtString.h"

BEG_VFS_NAMESPACE

void VirtualCtString::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/vfs_system/VirtualCtString.h" );
}

Str VirtualCtString::final_type_name_to_content( StrView str ) {
    auto i = str.find( "CtString<" );
    ASSERT( i != StrView::npos );
    return ctor_of_string_to_string( str.substr( i + 9, str.size() - ( i + 9 + 1 ) ) );
}

Str VirtualCtString::type_name() {
    return "VirtualCtString";
}

void VfsArgTrait<VirtualCtString>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualCtString &obj ) {
    Str type = "CtString<" + ctor_for( obj.value ) + ">";
    final_refs = { type + "();" };
    final_types = { type };
}

END_VFS_NAMESPACE
