#pragma once

#include "common_macros.h"

BEG_VFS_NAMESPACE

TUV requires requires ( U &a, V &&b ) { a.set( FORWARD( b ) ); }
void set( U &a, V &&b ) { a.set( FORWARD( b ) ); }

END_VFS_NAMESPACE
