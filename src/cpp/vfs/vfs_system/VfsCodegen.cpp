#include "../support/push_back_unique.h"
#include "../support/string/read_file.h"
#include "../support/check_dir.h"
#include "VfsSymbolCache.h"
#include "VfsCodegen.h"
#include <fstream>

BEG_VFS_NAMESPACE

VfsCodegen::VfsCodegen( VfsCodegen *parent ) : parent( parent ) {
    extra_cpp_flags = { "-std=c++20" };
    // second_phase = false;
    // return_type = nullptr;
}

void VfsCodegen::write_cpp_to( std::ostream &os ) {
    for( const auto &str : lib_names )
        os << "//// nsmake lib_name " << str << "\n";
    for( const auto &str : inc_paths )
        os << "//// nsmake inc_path " << str << "\n";
    if ( lib_names.size() || inc_paths.size() )
        os << "\n";

    for( const auto &str : inc_files ) {
        if ( str.starts_with( "inline:" ) ) {
            _write_inline_inc( os, str.substr( 7 ) );
            continue;
        }

        if ( str.starts_with( "./" ) )
            os << "#include \"" << str << "\"\n";
        else
            os << "#include <" << str << ">\n";
    }
    if ( init_lines.size() )
        os << "#include <vfs/support/OnInit.h>\n";

    _write_lines( os, prelimaries );

    _write_lines( os, lines );

    if ( init_lines.size() ) {
        os << "\n";
        os << "ON_INIT {";
        _write_lines( os, init_lines, 4 );
        os << "}\n";
    }
}

void VfsCodegen::get_link_flags_in( Vec<Str> &res ) {
    for( const auto &p : lib_paths )
        push_back_unique( res, "-L" + p );
    for( const auto &p : lib_names )
        push_back_unique( res, "-l" + p );
}

void VfsCodegen::get_cpp_flags_in( Vec<Str> &res ) {
    for( const auto &p : extra_cpp_flags )
        push_back_unique( res, p );
    for( const auto &p : inc_paths )
        push_back_unique( res, "-I" + p );
}

VfsSurdefStage VfsCodegen::invalid() {
    return { .result = VfsSurdefStage::Result::invalid };
}

VfsSurdefStage VfsCodegen::valid( Vec<double> pertinence ) {
    return { .pertinence = pertinence, .result = VfsSurdefStage::Result::valid };
}

Vec<Str> VfsCodegen::forwarded_args_from( PI start ) {
    Vec<Str> res;
    for( PI i = start; i < arg_names.size(); ++i )
        res.push_back( forwarded_arg( i ) );
    return res;
}

Vec<Str> VfsCodegen::forwarded_args() {
    return forwarded_args_from( 0 );
}

Str VfsCodegen::forwarded_arg( PI num ) {
    return "FORWARD( " + final_names[ num ] + " )";
}

void VfsCodegen::add_prel_block( const std::function<void (VfsCodegen &)> &f ) {
    VfsCodegen cg( this );
    f( cg );

    for( auto &line : cg.prelimaries )
        prelimaries.push_back( std::move( line ) );
    for( auto &line : cg.lines )
        prelimaries.push_back( std::move( line ) );
    for( auto &line : cg.init_lines )
        init_lines.push_back( std::move( line ) );
}

void VfsCodegen::_add_preliminary( const Str &str ) {
    if ( parent ) return parent->_add_preliminary( str );
    prelimaries.push_back( str );
}

void VfsCodegen::_add_init_line( const Str &str ) {
    if ( parent ) return parent->_add_init_line( str );
    init_lines.push_back( str );
}

void VfsCodegen::_add_cpp_flag( const Str &str ) { if ( parent ) return parent->_add_cpp_flag( str ); if ( str.size() ) push_back_unique( extra_cpp_flags, str ); }
void VfsCodegen::_add_inc_path( const Str &str ) { if ( parent ) return parent->_add_inc_path( str ); if ( str.size() ) push_back_unique( inc_paths, str ); }
void VfsCodegen::_add_inc_file( const Str &str ) { if ( parent ) return parent->_add_inc_file( str ); if ( str.size() ) push_back_unique( inc_files, str ); }
void VfsCodegen::_add_lib_flag( const Str &str ) { if ( parent ) return parent->_add_lib_flag( str ); if ( str.size() ) push_back_unique( lib_flags, str ); }
void VfsCodegen::_add_lib_path( const Str &str ) { if ( parent ) return parent->_add_lib_path( str ); if ( str.size() ) push_back_unique( lib_paths, str ); }
void VfsCodegen::_add_lib_name( const Str &str ) { if ( parent ) return parent->_add_lib_name( str ); if ( str.size() ) push_back_unique( lib_names, str ); }

void VfsCodegen::_add_line( const Str &str ) {
    lines.push_back( str );
}

void VfsCodegen::_write_inline_inc( std::ostream &os, const Str &str ) {
    // get filename
    std::hash<Str> str_hasher;
    for( auto hh = str_hasher( str ); ; ++hh ) {
        Str bn = std::to_string( hh ) + ".h";
        Str fn = vfs_symbol_source_directory / bn;
        Str cn = std::regex_replace( str, std::regex( "\\{INCLUDE_PATH\\}" ), "./" + bn );

        Opt<Str> cc = read_file( fn );
        if ( ! cc ) {
            check_dir( vfs_symbol_source_directory );
            std::ofstream fout( fn );
            if ( ! fout )
                ERROR( va_string( "Impossible to create file $0", fn ) );
            fout << cn;

            os << "#include \"" << bn << "\"\n";
            return;
        }

        if ( *cc == cn ) {
            os << "#include \"" << bn << "\"\n";
            return;
        }
    }
}

void VfsCodegen::_write_lines( std::ostream &os, const Vec<Str> &lines, int nsp ) {
    if ( ! lines.empty() )
        os << "\n";
    for( const auto &line : lines ) {
        if ( line.starts_with( "}" ) || line.starts_with( ")" ) )
            nsp = std::max( 0, nsp - 4 );

        for( int n = 0; n < nsp; ++n )
            os << " ";
        os << line << "\n";

        if ( line.ends_with( "{" ) || line.ends_with( "(" ) )
            nsp += 4;
    }
}

END_VFS_NAMESPACE
