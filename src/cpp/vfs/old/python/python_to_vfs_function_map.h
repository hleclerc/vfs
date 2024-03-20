#pragma once

#include <vfs/vfs_system/VirtualArgList.h>
#include "PyObject.h" // IWYU pragma: export
#include <map>

BEG_VFS_NAMESPACE

using VpiConversionFunc = void( VirtualArgList &ral, PyObject * );
extern std::map<Str,VpiConversionFunc *> python_to_vfs_function_map;

END_VFS_NAMESPACE
