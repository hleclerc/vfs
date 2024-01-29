#pragma once

#include "../support/get_compilation_flags_rec.h"
#include "../support/string/to_string.h"
#include "../support/CompilationFlags.h"
#include "../support/type_name.h"

BEG_VFS_NAMESPACE

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
        Seq<Str> seen;
        get_compilation_flags_rec( cf, seen, DECAYED_CT_OF( *arg ) );
        if constexpr ( requires { vfs_object_get_compilation_flags( cf, seen, *arg ); } )
            vfs_object_get_compilation_flags( cf, seen, *arg );

        // ct_key
        Str ct_key;
        if constexpr ( requires { vfs_object_ct_key( *arg ); } )
            ct_key = to_string( vfs_object_ct_key( *arg ) );

        //
        if constexpr ( requires { vfs_object_ct_cast( *arg ); } ) {
            Seq<Str> lcasts = vfs_object_ct_cast( *arg );
            add( arg, owned, VFS_NAMESPACE::type_name( DECAYED_CT_OF( *arg ) ), ct_key, &lcasts );
        } else {
            add( arg, owned, VFS_NAMESPACE::type_name( DECAYED_CT_OF( *arg ) ), ct_key, nullptr );
        }
    }
    
    void add( void *arg, bool owned, const Str &type_name, const Str &ct_key, const Seq<Str> *lcasts );

    static void get_compilation_flags( CompilationFlags &cn );
    static auto type_name() { return "RtArgList"; }
    
    Seq<Str>         type_names;
    Seq<void *>      pointers;
    Seq<Str>         casts;
    Str              keys;
    CompilationFlags cf;
};

void vfs_object_get_compilation_flags( CompilationFlags &cf, Seq<Str> &seen, const RtArgList &ral );
const Seq<Str> &vfs_object_ct_cast( const RtArgList &ral );
const Str &vfs_object_ct_key( const RtArgList &ral );

END_VFS_NAMESPACE
