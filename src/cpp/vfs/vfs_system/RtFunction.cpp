#include "../support/string/symbol_for.h"
#include "../support/string/ctor_for.h"
#include "RtFunction.h"

BEG_VFS_NAMESPACE
    
    void vfs_object_get_compilation_flags( CompilationFlags &cn, Seq<Str> &seen, const RtFunction &obj ) {
    cn << obj.cf;

    // make the function code,
    Str code = "inline:";
    Str sym = symbol_for( obj.name );
    code += "#include <vfs/support/CompilationFlags.h>\n";
    code += "\n";
    code += "BEG_VFS_NAMESPACE\n";
    code += "\n";
    code += "struct Function_" + sym + " {\n";
    code += "    static auto get_compilation_flags( CompilationFlags &cn ) { CompilationFlags tn( " + ctor_for( cn.flags ) + " ); cn << tn; cn.add_inc_file( \"{INCLUDE_PATH}\" ); }\n";
    code += "    static auto type_name            () { return \"Function_" + sym + "\"; }\n";
    if ( obj.name.ends_with( "__method" ) )
        code += "    auto        operator()           ( auto &&obj, auto &&...args ) const { return obj." + obj.name.substr( 0, obj.name.size() - 8 ) + "( FORWARD( args )... ); }\n";
    else
        code += "    auto        operator()           ( auto &&...args ) const { return " + obj.name + "( FORWARD( args )... ); }\n";
    code += "};\n";
    code += "\n";
    code += "END_VFS_NAMESPACE\n";

    // in an include file
    cn.add_inc_file( code );
}

const Str &vfs_object_ct_key( const RtFunction &obj ) {
    return obj.name;
}

Seq<Str> vfs_object_ct_cast( const RtFunction &obj ) {
    return { "auto {ARG} = Function_" + symbol_for( obj.name ) + "();" };
}

END_VFS_NAMESPACE
