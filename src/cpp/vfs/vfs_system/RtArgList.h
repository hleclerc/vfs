#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/string/to_string.h"
#include "../support/CompilationFlags.h"
#include "../support/type_name.h"

namespace Vfs {

///
class RtArgList {
public:
    void add_borrowed( auto *arg ) {
        add( arg, false );
    }

    void add_owned( auto *arg ) {
        add( arg, true );
    }

    void add( auto *arg, bool owned ) {
        // compilation flags
        Vec<Str> seen;
        get_compilation_flags_rec( cf, seen, DECAYED_CT_OF( *arg ) );
        if constexpr ( requires { vfs_object_get_compilation_flags( cf, seen, *arg ); } )
            vfs_object_get_compilation_flags( cf, seen, *arg );

        // ct_key
        Str ct_key;
        if constexpr ( requires { vfs_object_ct_key( *arg ); } )
            ct_key = to_string( vfs_object_ct_key( *arg ) );

        //
        if constexpr ( requires { vfs_object_ct_cast( *arg ); } ) {
            Vec<Str> lcasts = vfs_object_ct_cast( *arg );
            add( arg, owned, Vfs::type_name( DECAYED_CT_OF( *arg ) ), ct_key, &lcasts );
        } else {
            add( arg, owned, Vfs::type_name( DECAYED_CT_OF( *arg ) ), ct_key, nullptr );
        }
    }

    void add( void *arg, bool owned, const Str &type_name, const Str &ct_key, const Vec<Str> *lcasts );

    static void get_compilation_flags( CompilationFlags &cn );
    static auto type_name() { return "RtArgList"; }

    Vec<Str>         type_names;
    Vec<void *>      pointers;
    Vec<Str>         casts;
    Str              keys;
    CompilationFlags cf;
};

void vfs_object_get_compilation_flags( CompilationFlags &cf, Vec<Str> &seen, const RtArgList &ral );
const Vec<Str> &vfs_object_ct_cast( const RtArgList &ral );
const Str &vfs_object_ct_key( const RtArgList &ral );

} // namespace Vfs
