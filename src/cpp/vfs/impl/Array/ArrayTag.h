#pragma once

//#include "../containers/Memory/Memory_Cpu.h"
// #include "../../support/CompilationFlags.h"
#include "../../support/common_macros.h"

BEG_VFS_NAMESPACE

namespace ArrayTag {
    #define DECL_TAG_T( NAME ) TT struct NAME { using value = T; static auto template_type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtType<T>() ); } }
    #define DECL_TAG_i( NAME ) Ti struct NAME { static constexpr int value = i; static auto template_type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } static void for_each_template_arg( auto &&f ) { f( CtInt<i>() ); } }
    #define DECL_TAG__( NAME ) struct NAME { static auto type_name() { return "VFS_NAMESPACE::ArrayTag::" #NAME; } }
        DECL_TAG__( WantCtSizeInAllDims );
        DECL_TAG__( ForceKnownSizes );
        DECL_TAG__( ForceTupleRepr );
        DECL_TAG_i( ForceNbDimsTo );
    #undef DECL_TAG_T
    #undef DECL_TAG_i
    #undef DECL_TAG__
} // namespace ArrayTag

END_VFS_NAMESPACE
