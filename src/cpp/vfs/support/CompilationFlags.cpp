#include "../support/size_and_lexical_comparison.h"
#include "../support/push_back_unique.h"
#include "../support/string/ctor_for.h"
#include "../vfs_system/VfsCodegen.h"
#include "CompilationFlags.h"

BEG_VFS_NAMESPACE

static bool cmp_by_flag_type( const Str &flag_a, const Str &flag_b ) {
    return flag_a.size() >= 8 && flag_b.size() >= 8 ? flag_a.substr( 0, 8 ) < flag_b.substr( 0, 8 ) : flag_a < flag_b;
}

CompilationFlags::CompilationFlags( const Seq<Str> &flags ) : flags( flags ) {
}

CompilationFlags &CompilationFlags::operator<<( const CompilationFlags &that ) {
    for( const auto &flag : that.flags )
        push_back_unique( flags, flag );
    sort();
    return *this;
}

bool CompilationFlags::operator<( const CompilationFlags &that ) const {
    return size_and_lexical_comparison( flags, that.flags );
}

void CompilationFlags::add_flags_to( VfsCodegen &cg ) const {
    for( const Str &flag : flags ) {
        if ( flag.starts_with( "cpp_flag:" ) ) { cg.add_cpp_flag( flag.substr( 9 ) ); continue; }
        if ( flag.starts_with( "cpp_file:" ) ) { TODO; continue; }
        if ( flag.starts_with( "inc_path:" ) ) { cg.add_inc_path( flag.substr( 9 ) ); continue; }
        if ( flag.starts_with( "inc_file:" ) ) { cg.add_inc_file( flag.substr( 9 ) ); continue; }
        if ( flag.starts_with( "lib_flag:" ) ) { cg.add_lib_flag( flag.substr( 9 ) ); continue; }
        if ( flag.starts_with( "lib_path:" ) ) { cg.add_lib_path( flag.substr( 9 ) ); continue; }
        if ( flag.starts_with( "lib_name:" ) ) { cg.add_lib_name( flag.substr( 9 ) ); continue; }
        PE( flag );
        ERROR( "not a know flag" );
    }
}

DisplayItem *CompilationFlags::display( Displayer &ds ) const {
    return DS_OBJECT( CompilationFlags, flags );
}

void CompilationFlags::add_cpp_flag( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "cpp_flag:" + val ); sort(); }
void CompilationFlags::add_cpp_file( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "cpp_file:" + val ); sort(); }
void CompilationFlags::add_inc_file( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "inc_file:" + val ); sort(); }
void CompilationFlags::add_inc_path( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "inc_path:" + val ); sort(); }
void CompilationFlags::add_lib_flag( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "lib_flag:" + val ); sort(); }
void CompilationFlags::add_lib_name( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "lib_name:" + val ); sort(); }
void CompilationFlags::add_lib_path( const Str &val ) { if ( val.empty() ) return; push_back_unique( flags, "lib_path:" + val ); sort(); }

void CompilationFlags::sort() {
    std::sort( flags.begin(), flags.end(), cmp_by_flag_type );
}

Str ctor_for( const CompilationFlags &cn ) {
    return "{ " + ctor_for( cn.flags ) + " }";
}

RtStringList ct_value( const CompilationFlags &cn ) {
    return { cn.flags };
}


END_VFS_NAMESPACE
