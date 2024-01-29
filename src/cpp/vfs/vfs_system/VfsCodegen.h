#pragma once

#include "../support/string/va_string.h"
#include "../support/string/join.h" // IWYU pragma: export
#include "../support/TypeConfig.h"
#include "VfsSurdefStage.h"
#include <functional>

BEG_VFS_NAMESPACE
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

    void           get_link_flags_in  ( Seq<Str> &link_flags );
    void           get_cpp_flags_in   ( Seq<Str> &cpp_flags );
    void           write_cpp_to       ( std::ostream &os );

    // helpers
    VfsSurdefStage invalid            ();
    VfsSurdefStage valid              ( Seq<double> pertinence = { 0.0 } );
    
    Seq<Str>       forwarded_args_from( PI start );
    Seq<Str>       forwarded_args     ();
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
    void           _write_lines     ( std::ostream &os, const Seq<Str> &lines, int nsp = 0 );

    Seq<Str>       extra_cpp_flags; ///<
    Seq<Str>       prelimaries;
    Seq<Str>       init_lines;      ///< ON_INIT ...
    Seq<Str>       inc_paths;
    Seq<Str>       inc_files;
    Seq<Str>       lib_flags;
    Seq<Str>       lib_paths;
    Seq<Str>       lib_names;
    Seq<Str>       lines;

    VfsCodegen*    parent;

    Str            function_name;
    Str            return_type;
    Seq<Str>       arg_types;
    Seq<Str>       arg_names;
    Seq<Str>       arg_casts;
    Seq<Str>       props;
    Seq<Str>       parms;
};

END_VFS_NAMESPACE
