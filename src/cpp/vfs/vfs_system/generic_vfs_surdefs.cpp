#include "../support/string/decay.h"
#include "VfsSymbolCache.h"
#include "gmpxx.h"

BEG_VFS_NAMESPACE

ON_INIT {
    // generic forwarders -------------------------------------------------------------------------------------------------------------------------------------------------
    // forward call
    VFS_ADD_SURDEF( "*" ) {
        cg.add_line( "return $0( $1 );", cg.func_name, join( cg.forwarded_args() ) );
        return cg.valid( { -1e6 } );
    };

    // forward method call
    VFS_ADD_SURDEF( "*__method" ) {
        cg.add_line( "return $0.$1( $2 );", cg.final_names[ 0 ], cg.func_name.substr( 0, cg.func_name.size() - 8 ), join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid( { -1e6 + 1 } );
    };

    // forward method call with _cast instead of final_ref
    VFS_ADD_SURDEF( "*__cast_method" ) {
        cg.add_line( "return $0.$1( $2 );", cg.cast_names[ 0 ], cg.func_name.substr( 0, cg.func_name.size() - 13 ), join( cg.forwarded_args_from( cg.nb_final_refs_from_cast[ 0 ] ) ) );
        return cg.valid( { -1e6 + 1 } );
    };

    // call( func, args... )
    VFS_ADD_SURDEF( "call", "func" ) {
        // forward args
        cg.add_line( "return func( $0 );", join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid();
    };

    // call_to_Void_if_void( func, args... ) => return a Void() if func does not return anything. Else, return what func gives to us
    VFS_ADD_SURDEF( "call_to_Void_if_void", "func" ) {
        // intermediate function to replace an inline `if constexpr ( std::is_same_v<void,...> )` that does not work as expected (at least for g++-13.2)
        cg.add_prel_block( [&]( VfsCodegen &cg ) {
            cg.add_line( "auto __apply( CtInt<0>, auto &&func, auto &&...args ) {" );
            cg.add_line( "return func( FORWARD( args )... );" );
            cg.add_line( "}" );
            cg.add_line( "" );
            cg.add_line( "auto __apply( CtInt<1>, auto &&func, auto &&...args ) {" );
            cg.add_line( "func( FORWARD( args )... );" );
            cg.add_line( "return Void{};" );
            cg.add_line( "}" );
        } );

        // forward args
        cg.add_line( "using Ret = DECAYED_TYPE_OF( func( $0 ) );", join( cg.forwarded_args_from( 1 ) ) );
        cg.add_line( "constexpr bool ret_void = std::is_same_v<Ret,void>;" );
        cg.add_line( "return __apply( CtInt<ret_void>(), FORWARD( func ), $0 );", join( cg.forwarded_args_from( 1 ) ) );
        return cg.valid();
    };

    //
    VFS_ADD_SURDEF( "call_by_name", "name", "a", "b" ) {
        if ( cg.final_types[ 0 ] == "CtString<\"div\">" ) {
            auto integral_type = []( const Str &type ) {
                return type == "SI8"  || type == "SI16" || type == "SI32" || type == "SI64" ||
                       type == "PI8"  || type == "PI16" || type == "PI32" || type == "PI64" ;
            };
            if ( cg.final_types.size() == 3 && integral_type( cg.final_types[ 1 ] ) && integral_type( cg.final_types[ 2 ] ) ) {
                cg.add_inc_file( "vfs/support/RationalGmp.h" );
                cg.add_lib_name( "gmpxx" );
                cg.add_lib_name( "gmp" );

                cg.add_line( "return RationalGmp( a, b );" );
                return cg.valid();
            }
        }

        return cg.invalid();
    };


    // vfs td handling ----------------------------------------------------------------------------------------------------------------------------------------------------
    // vfs_td_reassign
    VFS_ADD_SURDEF( "vfs_td_reassign", "self", "that" ) {
        // if same types
        if ( cg.final_types[ 0 ] == cg.final_types[ 1 ] ) {
            cg.add_line( "self = FORWARD( that );" );
            return cg.valid();
        }

        cg.add_line( "self_decl.~$0();", decay( cg.arg_types[ 0 ] ) );
        cg.add_line( "new ( &self_decl ) $0( FORWARD( that ) );", decay( cg.arg_types[ 0 ] ) );
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
