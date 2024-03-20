#include "../support/string/symbol_for.h"
#include "../support/string/ctor_for.h"
#include "VirtualFunction.h"

BEG_VFS_NAMESPACE

void VirtualFunction::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/vfs_system/VirtualFunction.h" );
}

Str VirtualFunction::type_name() {
    return "VirtualFunction";
}

int VirtualFunction::compare( const VirtualFunction &that ) const {
    return compare_chain( name, that.name, cf, that.cf );
}

void VfsArgTrait<VirtualFunction>::get_cg_data( CompilationFlags &cf, Vec<Str> &seen_for_cf, Str &cast_type, Vec<Str> &final_types, Vec<Str> &final_refs, const VirtualFunction &obj ) {
    cf << obj.cf;

    // make the function code,
    Str code = "inline:";
    Str sym = symbol_for( obj.name );
    code += "#include <vfs/support/CompilationFlags.h>\n";
    code += "\n";
    code += "BEG_VFS_NAMESPACE\n";
    code += "\n";
    code += "struct Function_" + sym + " {\n";
    code += "    static auto get_compilation_flags( CompilationFlags &cn ) { CompilationFlags tn( " + ctor_for( cf.flags ) + " ); cn << tn; cn.add_inc_file( \"{INCLUDE_PATH}\" ); }\n";
    code += "    static auto type_name            () { return \"Function_" + sym + "\"; }\n";
    if ( obj.name.ends_with( "__method" ) )
        code += "    auto        operator()           ( auto &&obj, auto &&...args ) const { return obj." + obj.name.substr( 0, obj.name.size() - 8 ) + "( FORWARD( args )... ); }\n";
    else
        code += "    auto        operator()           ( auto &&...args ) const { return " + obj.name + "( FORWARD( args )... ); }\n";
    code += "};\n";
    code += "\n";
    code += "END_VFS_NAMESPACE\n";

    // in an include file
    cf.add_inc_file( code );

    Str type = "Function_" + symbol_for( obj.name );
    final_refs = { type + "()" };
    final_types = { type };
}


END_VFS_NAMESPACE
