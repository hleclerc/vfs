#pragma once

#include "CompilationFlags.h"
#include <gmpxx.h>

BEG_VFS_NAMESPACE

using        Rational             = mpq_class;

inline auto  get_compilation_flags( CompilationFlags &cf, CtType<Rational> ) { cf.add_inc_file( "vfs/support/Rational_gmp.h" ); }
inline auto  type_name            ( CtType<Rational> ) { return "Rational"; }
inline auto *display              ( Displayer &ds, const Rational &q ) { return ds.number( q.get_str() ); }

END_VFS_NAMESPACE
