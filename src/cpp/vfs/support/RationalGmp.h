#pragma once

#include "CompilationFlags.h"
#include <gmpxx.h>

BEG_VFS_NAMESPACE

using        RationalGmp          = mpq_class;

inline auto  get_compilation_flags( CompilationFlags &cf, CtType<RationalGmp> ) { cf.add_inc_file( "vfs/support/RationalGmp.h" ); }
inline auto  type_name            ( CtType<RationalGmp> ) { return "RationalGmp"; }
inline auto *display              ( Displayer &ds, const RationalGmp &q ) { return ds.number( q.get_str() ); }

END_VFS_NAMESPACE
