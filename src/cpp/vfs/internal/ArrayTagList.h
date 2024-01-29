#pragma once

#include "../containers/Memory/Memory_Cpu.h"
#include "../Type.h"

BEG_VFS_NAMESPACE

// Tag types ----------------------------------------------------------------------------------------------------------------------------------------------
namespace ArrayTag {
    #define DECL_TAG_T( NAME ) TT struct NAME { static auto template_type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtType<T>() ); } }
    #define DECL_TAG_i( NAME ) Ti struct NAME { static auto template_type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtInt<i>() ); } }
    #define DECL_TAG__( NAME ) struct NAME { static auto type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } }
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

///
template<class... Tags>
class ArrayTagList {
public:
    static constexpr auto want_ct_size_in_dim  ( auto dim ) { return CtTypeList<Tags...>::reduction( [&]( auto tag, auto res ) { return ArrayTag::want_ct_size_in( tag, dim, res ); }, CtInt<0>() ); }
    static constexpr auto want_nb_dims         () { return CtTypeList<Tags...>::reduction( [&]( auto tag, auto res ) { return ArrayTag::want_nb_dims( tag, res ); }, CtInt<1>() ); }

    static void           for_each_template_arg( auto &&f ) { ( f( CtType<Tags>() ), ... ); }
    static void           get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/internal/ArrayTagList.h" ); }
    static auto           template_type_name   () { return "VFS_NAMESPACE::ArrayTagList"; }

    static auto           array_type_for       ( auto obj_type, auto item_type, auto &&size ) {
        // template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
        if constexpr ( want_ct_size_in_dim( CtInt<0>() ) ) {
            return Type( "Vec", "inc_file:vfs/containers/Vec.h", item_type, size       , CtInt<0>(), CtInt<0>(), CtInt<0>() );
        } else {
            return Type( "Vec", "inc_file:vfs/containers/Vec.h", item_type, CtInt<-1>(), CtInt<0>(), CtInt<0>(), CtInt<1>() );
        }
    }

};

END_VFS_NAMESPACE
