#pragma once

#include "../vfs_namespace.h"
#include <string_view>

BEG_VFS_NAMESPACE

std::string_view read_arg_name( std::string_view &arg_names );

END_VFS_NAMESPACE
