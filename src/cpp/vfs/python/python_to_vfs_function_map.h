#pragma once

#include <vfs/vfs_system/RtArgList.h>
#include "PyObject.h" // IWYU pragma: export
#include <map>

BEG_VFS_NAMESPACE

using VpiConversionFunc = void( RtArgList &ral, PyObject * );
extern std::map<Str,VpiConversionFunc *> python_to_vfs_function_map;

END_VFS_NAMESPACE
