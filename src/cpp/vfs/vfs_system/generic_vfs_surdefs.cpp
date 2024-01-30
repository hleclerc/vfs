#include "VfsSymbolCache.h"

BEG_VFS_NAMESPACE

ON_INIT {
    // generic forwarders -------------------------------------------------------------------------------------------------------------------------------------------------
    // forward call
    VFS_ADD_SURDEF( ".*" ) {
        cg.add_line( "return $0( $1 );", cg.function_name, join( cg.forwarded_args() ) );
        return cg.valid( { -1e6 } );
    };

    // forward method call
    VFS_ADD_SURDEF( ".*__method" ) {
        cg.add_line( "return $0.$1( $2 );", cg.arg_names[ 0 ], cg.function_name.substr( 0, cg.function_name.size() - 8 ), join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid( { -1e6 + 1 } );
    };

    // call( func, args... )
    VFS_ADD_SURDEF( "call", "func" ) {
        // intermediate function to replace an inline `if constexpr ( std::is_same_v<void,...> )` that does not work (at least for g++-13.2)
        cg.add_prel_block( [&]( VfsCodegen &cg ) {
            cg.add_line( "auto __apply( CtInt<0>, const auto &func, auto &&...args ) {" );
            cg.add_line( "return func( FORWARD( args )... );" );
            cg.add_line( "}" );
            cg.add_line( "auto __apply( CtInt<1>, const auto &func, auto &&...args ) {" );
            cg.add_line( "func( FORWARD( args )... );" );
            cg.add_line( "return Void{};" );
            cg.add_line( "}" );
        } );

        // forward args
        cg.add_line( "return __apply( CtInt<std::is_same_v<void,decltype( func( $0 ) )>>{}, $1 );", join( cg.forwarded_args_from( 1 ) ), join( cg.forwarded_args() ) );
        return cg.valid();
    };

    // vfs td handling ----------------------------------------------------------------------------------------------------------------------------------------------------
    // vfs_td_reassign
    VFS_ADD_SURDEF( "vfs_td_reassign", "self", "that" ) {
        cg.add_line( "self_decl._vfs_type_and_data.destroy( CT_DECAYED_TYPE_OF( self ) );" );
        cg.add_line( "self_decl._vfs_type_and_data.construct( FromValue(), FORWARD( that ) );" );
        return cg.valid();
    };

    // vfs_td_destroy
    VFS_ADD_SURDEF( "vfs_td_destroy", "self" ) {
        cg.add_line( "self_decl._vfs_type_and_data.destroy( CT_DECAYED_TYPE_OF( self ) );" );
        return cg.valid();
    };
}

END_VFS_NAMESPACE
