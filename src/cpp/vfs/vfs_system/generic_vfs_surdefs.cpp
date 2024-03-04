#include "VfsSymbolCache.h"

BEG_VFS_NAMESPACE

ON_INIT {
    // generic forwarders -------------------------------------------------------------------------------------------------------------------------------------------------
    // forward call
    VFS_ADD_SURDEF( ".*" ) {
        cg.add_line( "return $0( $1 );", cg.func_name, join( cg.forwarded_args() ) );
        return cg.valid( { -1e6 } );
    };

    // forward method call
    VFS_ADD_SURDEF( ".*__method" ) {
        cg.add_line( "return $0.$1( $2 );", cg.final_names[ 0 ], cg.func_name.substr( 0, cg.func_name.size() - 8 ), join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid( { -1e6 + 1 } );
    };

    // forward method call with _cast instead of final_ref
    VFS_ADD_SURDEF( ".*__cast_method" ) {
        cg.add_line( "return $0.$1( $2 );", cg.cast_names[ 0 ], cg.func_name.substr( 0, cg.func_name.size() - 13 ), join( cg.forwarded_args_from( cg.nb_final_refs_from_cast[ 0 ] ) ) );
        return cg.valid( { -1e6 + 1 } );
    };

    // call( func, args... )
    VFS_ADD_SURDEF( "call", "func" ) {
        // intermediate function to replace an inline `if constexpr ( std::is_same_v<void,...> )` that does not work as expected (at least for g++-13.2)
        // cg.add_prel_block( [&]( VfsCodegen &cg ) {
        //     cg.add_line( "auto __apply( CtInt<0>, const auto &func, auto &&...args ) {" );
        //     cg.add_line( "return func( FORWARD( args )... );" );
        //     cg.add_line( "}" );
        //     cg.add_line( "" );
        //     cg.add_line( "auto __apply( CtInt<1>, const auto &func, auto &&...args ) {" );
        //     cg.add_line( "func( FORWARD( args )... );" );
        //     cg.add_line( "return Void{};" );
        //     cg.add_line( "}" );
        // } );

        // forward args
        //cg.add_line( "return __apply( CtInt<std::is_same_v<void,decltype( func( $0 ) )>>{}, $1 );", join( cg.forwarded_args_from( 1 ) ), join( cg.forwarded_args() ) );
        cg.add_line( "return func( $0 );", join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid();
    };

    // vfs td handling ----------------------------------------------------------------------------------------------------------------------------------------------------
    // vfs_td_reassign
    VFS_ADD_SURDEF( "vfs_td_reassign", "self", "that" ) {
        // if same types
        if ( cg.final_types[ 0 ] == cg.final_types[ 1 ] ) {
            cg.add_line( "self = FORWARD( that );" );
            return cg.valid();
        }

        cg.add_line( "self_decl = FORWARD( that_decl );" );
        return cg.valid();
    };

    // vfs_td_destroy => destroy the _decl variable
    // VFS_ADD_SURDEF( "destroy", "self" ) {
    //     cg.add_line( "using T = std::decay_t<decltype( self_cast )>;" );
    //     cg.add_line( "self_cast.~T();" );
    //     return cg.valid( { 0 } );
    // };
}

END_VFS_NAMESPACE
