#pragma once

#include "CompilationFlags.h"
#include <boost/rational.hpp>

BEG_VFS_NAMESPACE

    using RationalGmp = boost::rational<int>;

inline auto  get_compilation_flags( CompilationFlags &cf, CtType<RationalGmp> ) { cf.add_inc_file( "vfs/support/Rational.h" ); }
inline auto  type_name            ( CtType<RationalGmp> ) { return "Rational"; }

END_VFS_NAMESPACE
