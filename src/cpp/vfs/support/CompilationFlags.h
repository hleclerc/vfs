#pragma once

#include "../vfs_system/RtStringList.h"

namespace Vfs {
class VfsCodegen;

///
class CompilationFlags {
public:
    /***/        CompilationFlags( const Vec<Str> &flags = {} );

    auto         operator<<      ( const CompilationFlags &that ) -> CompilationFlags &;
    bool         operator<       ( const CompilationFlags &that ) const;

    void         add_flags_to    ( VfsCodegen &cg ) const;
    DisplayItem* display         ( Displayer &ds ) const;

    void         add_cpp_flag    ( const Str &val );
    void         add_cpp_file    ( const Str &val );
    void         add_inc_file    ( const Str &val );
    void         add_inc_path    ( const Str &val );
    void         add_lib_flag    ( const Str &val );
    void         add_lib_name    ( const Str &val );
    void         add_lib_path    ( const Str &val );

    void         sort            ();

    Vec<Str>     flags;          ///< like "inc_file:..."
};

RtStringList ct_value( const CompilationFlags &cn );
Str          ctor_for( const CompilationFlags &cn );

// by default, call T::get_compilation_flags( res )
TT auto get_compilation_flags( CompilationFlags &res, CtType<T> ) -> decltype( T::get_compilation_flags( res ) ) { return T::get_compilation_flags( res ); }

} // namespace Vfs
