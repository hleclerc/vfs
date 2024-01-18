#pragma once

#include "../support/containers/Memory/Memory_Cpu.h"
#include "../Type.h"

namespace Vfs {

namespace ArrayTag {
    #define DECL_TAG_T( NAME ) TT struct NAME { static auto template_type_name() { return "Vfs::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtType<T>() ); } }
    #define DECL_TAG_i( NAME ) Ti struct NAME { static auto template_type_name() { return "Vfs::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtInt<i>() ); } }
    #define DECL_TAG__( NAME ) struct NAME { static auto type_name() { return "Vfs::ArrayTag::" #NAME; } }
    DECL_TAG__( WantCtSizeInAllDims );
    DECL_TAG_i( ForceNbDimsTo );
    #undef DECL_TAG_T
    #undef DECL_TAG_i
    #undef DECL_TAG__

    constexpr auto want_ct_size_in( CtType<WantCtSizeInAllDims>, auto dim, auto res ) { return CtInt<1>(); }
    constexpr auto want_ct_size_in( auto tag, auto dim, auto res ) { return res; }

    Ti constexpr auto want_nb_dims( CtType<ForceNbDimsTo<i>>, auto res ) { return CtInt<i>(); }
    constexpr auto want_nb_dims( auto tag, auto res ) { return res; }
}

template<class... Tags>
class ArrayTagList {
public:
    static constexpr auto want_ct_size_in_dim  ( auto dim ) { return CtTypeList<Tags...>::reduction( [&]( auto tag, auto res ) { return ArrayTag::want_ct_size_in( tag, dim, res ); }, CtInt<0>() ); }
    static constexpr auto want_nb_dims         () { return CtTypeList<Tags...>::reduction( [&]( auto tag, auto res ) { return ArrayTag::want_nb_dims( tag, res ); }, CtInt<1>() ); }

    static void           for_each_template_arg( auto &&f ) { ( f( CtType<Tags>() ), ... ); }
    static void           get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/internal/ArrayTagList.h" ); }
    static auto           template_type_name   () { return "ArrayTagList"; }
    static auto           memory               () { return Memory_Cpu(); }

    static auto           array_type_for       ( auto item_type, auto &&size ) {
        // template<class Item,class Memory,int static_size,int alignment,int local_size,bool allow_heap>
        if constexpr ( want_ct_size_in_dim( CtInt<0>() ) ) {
            return Type( "GenericVector", "inc_file:vfs/support/containers/GenericVector.h", item_type, CtType<Memory_Cpu>(), size, CtInt<0>(), CtInt<0>(), CtInt<0>() );
        } else {
            return Type( "GenericVector", "inc_file:vfs/support/containers/GenericVector.h", item_type, CtType<Memory_Cpu>(), CtInt<-1>(), CtInt<0>(), CtInt<0>(), CtInt<1>() );
        }
    }

};

}
