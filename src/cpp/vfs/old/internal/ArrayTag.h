#pragma once

//#include "../containers/Memory/Memory_Cpu.h"
#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

///
// template<class... Tags>
// class ArrayTagList {
// public:
//     static void for_each_template_arg( auto &&f ) { ( f( CtType<Tags>() ), ... ); }
//     static void get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/internal/ArrayTagList.h" ); }
//     static auto template_type_name   () { return "VFS_NAMESPACE::ArrayTagList"; }
// };

// Tag types ----------------------------------------------------------------------------------------------------------------------------------------------
namespace ArrayTag {
    #define DECL_TAG_T( NAME ) TT struct NAME { using value = T; static auto template_type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtType<T>() ); } }
    #define DECL_TAG_i( NAME ) Ti struct NAME { static constexpr int value = i; static auto template_type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtInt<i>() ); } }
    #define DECL_TAG__( NAME ) struct NAME { static auto type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } }
        DECL_TAG__( WantCtSizeInAllDims );
        DECL_TAG_i( ForceNbDimsTo );
    #undef DECL_TAG_T
    #undef DECL_TAG_i
    #undef DECL_TAG__
} // namespace ArrayTag

END_VFS_NAMESPACE
