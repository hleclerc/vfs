#pragma once

#include "../support/string/va_string.h"
#include "../support/string/join.h" // IWYU pragma: export
#include "../support/TypeConfig.h"
#include "VfsSurdefStage.h"
#include <functional>

namespace Vfs {
class VfsTypeInfo;

///
class VfsCodegen {
public:
    /**/           VfsCodegen         ( VfsCodegen *parent = nullptr );

    void           add_preliminary    ( auto &&...args ) { return _add_preliminary( repl( FORWARD( args )... ) ); }
    void           add_init_line      ( auto &&...args ) { return _add_init_line  ( repl( FORWARD( args )... ) ); }
    void           add_cpp_flag       ( auto &&...args ) { return _add_cpp_flag   ( repl( FORWARD( args )... ) ); }
    void           add_inc_path       ( auto &&...args ) { return _add_inc_path   ( repl( FORWARD( args )... ) ); }
    void           add_inc_file       ( auto &&...args ) { return _add_inc_file   ( repl( FORWARD( args )... ) ); }
    void           add_lib_flag       ( auto &&...args ) { return _add_lib_flag   ( repl( FORWARD( args )... ) ); }
    void           add_lib_path       ( auto &&...args ) { return _add_lib_path   ( repl( FORWARD( args )... ) ); }
    void           add_lib_name       ( auto &&...args ) { return _add_lib_name   ( repl( FORWARD( args )... ) ); }
    void           add_line           ( auto &&...args ) { return _add_line       ( repl( FORWARD( args )... ) ); }
    auto           repl               ( auto &&...args ) { return va_string       ( FORWARD( args )... ); }
    
    void           add_prel_block     ( const std::function<void( VfsCodegen &cg )> &f );

    void           get_link_flags_in  ( Vec<Str> &link_flags );
    void           get_cpp_flags_in   ( Vec<Str> &cpp_flags );
    void           write_cpp_to       ( std::ostream &os );

    // helpers
    VfsSurdefStage invalid            ();
    VfsSurdefStage valid              ( Vec<double> pertinence = { 0.0 } );
    
    Vec<Str>       forwarded_args_from( PI start );
    Vec<Str>       forwarded_args     ();
    Str            forwarded_arg      ( PI num );
    Str            real_type          ( PI num );


    void           _add_preliminary ( const Str &str );
    void           _add_init_line   ( const Str &str );
    void           _add_cpp_flag    ( const Str &str );
    void           _add_inc_path    ( const Str &str );
    void           _add_inc_file    ( const Str &str );
    void           _add_lib_flag    ( const Str &str );
    void           _add_lib_path    ( const Str &str );
    void           _add_lib_name    ( const Str &str );
    void           _add_line        ( const Str &str );

    void           _write_inline_inc( std::ostream &os, const Str &str );
    void           _write_lines     ( std::ostream &os, const Vec<Str> &lines, int nsp = 0 );

    Vec<Str>       extra_cpp_flags; ///<
    Vec<Str>       prelimaries;
    Vec<Str>       init_lines;      ///< ON_INIT ...
    Vec<Str>       inc_paths;
    Vec<Str>       inc_files;
    Vec<Str>       lib_flags;
    Vec<Str>       lib_paths;
    Vec<Str>       lib_names;
    Vec<Str>       lines;

    VfsCodegen*    parent;

    Str            function_name;
    Str            return_type;
    Vec<Str>       arg_types;
    Vec<Str>       arg_names;
    Vec<Str>       arg_casts;
    Vec<Str>       props;
    Vec<Str>       parms;
};

}
