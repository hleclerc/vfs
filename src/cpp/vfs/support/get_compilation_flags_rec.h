#pragma once

#include "CompilationFlags.h"
#include "type_name.h"

BEG_VFS_NAMESPACE

///
TT void get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen, CtType<T> ) {
    // avoid infinite recursion
    auto name = type_name<T>();
    if ( std::find( seen.begin(), seen.end(), name ) != seen.end() )
        return;
    seen.push_back( name );

    // recursion
    if constexpr ( requires { for_each_template_arg( CtType<T>(), []( auto ) {} ); } ) {
        for_each_template_arg( CtType<T>(), [&]( auto a ) {
            if constexpr ( IsA_CtType<decltype( a )> )
                get_compilation_flags_rec( res, seen, a );
        } );
    }

    // compilations needs for T
    if constexpr ( requires { get_compilation_flags( res, CtType<T>() ); } )
        get_compilation_flags( res, CtType<T>() );
}

// refs
TT void get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen, CtType<const T *> ) { get_compilation_flags_rec( res, seen, CtType<T>() ); }
TT void get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen, CtType<const T &> ) { get_compilation_flags_rec( res, seen, CtType<T>() ); }
TT void get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen, CtType<T &&> ) { get_compilation_flags_rec( res, seen, CtType<T>() ); }
TT void get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen, CtType<T &> ) { get_compilation_flags_rec( res, seen, CtType<T>() ); }
TT void get_compilation_flags_rec( CompilationFlags &res, Vec<Str> &seen, CtType<T *> ) { get_compilation_flags_rec( res, seen, CtType<T>() ); }

END_VFS_NAMESPACE
