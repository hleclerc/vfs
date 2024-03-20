#pragma once

#include "../vfs_system/VirtualCtStringList.h"

BEG_VFS_NAMESPACE
class VfsCodegen;

///
class CompilationFlags {
public:
    // /***/       CompilationFlags( const std::initializer_list<Str> &flags );
    /***/          CompilationFlags( const Vec<Str> &flags = {} );

    auto           operator<<      ( const CompilationFlags &that ) -> CompilationFlags &;
    auto           operator<<      ( const Str &flag ) -> CompilationFlags &;

    void           add_flags_to    ( VfsCodegen &cg ) const;
    SI             compare         ( const CompilationFlags &that ) const;
    DisplayItem*   display         ( Displayer &ds ) const;

    void           add_cpp_flag    ( const Str &val );
    void           add_cpp_file    ( const Str &val );
    void           add_inc_file    ( const Str &val );
    void           add_inc_path    ( const Str &val );
    void           add_lib_flag    ( const Str &val );
    void           add_lib_name    ( const Str &val );
    void           add_lib_path    ( const Str &val );

    void           sort            ();

    Vec<Str>       flags;          ///< like "inc_file:..."
};

VirtualCtStringList ct_value( const CompilationFlags &cn );
Str                 ctor_for( const CompilationFlags &cn );

// by default, call T::get_compilation_flags( res )
TT auto get_compilation_flags( CompilationFlags &res, CtType<T> ) -> decltype( T::get_compilation_flags( res ) ) { return T::get_compilation_flags( res ); }

END_VFS_NAMESPACE
