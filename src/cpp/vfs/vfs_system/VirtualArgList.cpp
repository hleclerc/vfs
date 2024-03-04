#include "../support/string/va_string.h"
#include "../support/string/join.h"
#include "VirtualArgList.h"
#include "VfsFunc.h"
//#include <regex>

BEG_VFS_NAMESPACE

VirtualArgList::~VirtualArgList() {
    VFS_CALL_METHOD( destroy, CtStringList<>, void, *this );
}

void VfsArgTrait<VirtualArgList>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualArgList &obj ) {
    cf.add_inc_file( "vfs/vfs_system/VirtualArgList_.h" );
    cf << obj.key.cf;

    Vec<Str> ptr_types;
    for( const VirtualArgList::TypeData &ad : obj.key.type_data ) {
        Str cast_type = ad.cast_type.size() ? ad.cast_type : ad.arg_type;
        ptr_types << va_string( "$0 *", cast_type );
        for( const Str &final_type : ad.final_types )
            final_types << final_type;
        for( const Str &final_ref : ad.final_refs )
            final_refs << final_ref;
    }

    cast_type = va_string( "VirtualArgList_<$0>", join( ptr_types ) );
}

const VirtualArgList::Key &VfsArgTrait<VirtualArgList>::key( const VirtualArgList &obj ) {
    return obj.key;
}

void VirtualArgList::get_compilation_flags(CompilationFlags &cn) {
    cn.add_inc_file( "vfs/vfs_system/VirtualArgList.h" );
}

Str VirtualArgList::type_name() {
    return "VirtualArgList";
}

SI VirtualArgList::TypeData::compare( const TypeData &that ) const {
    return compare_chain(
        final_types, that.final_types,
        final_refs, that.final_refs,
        cast_type, that.cast_type,
        arg_type, that.arg_type,
        key, that.key
    );
}

SI VirtualArgList::Key::compare( const Key &that) const {
    return compare_chain(
        type_data, that.type_data,
        cf, that.cf
    );
}

END_VFS_NAMESPACE


