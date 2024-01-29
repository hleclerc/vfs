#pragma once

#include "string/va_string.h"
#include <filesystem>
#include "ERROR.h"

BEG_VFS_NAMESPACE

inline void check_dir( const std::filesystem::path &path ) {
    if ( ! std::filesystem::exists( path ) ) {
        std::error_code ec;
        std::filesystem::create_directories( path, ec );
        if ( ec )
            ERROR( va_string( "Impossible to create vfs cache directory '$0': $1", path.string(), ec.message() ) );
    }

    if ( ! std::filesystem::is_directory( path ) )
        ERROR( va_string( "'$0' is not a directory (but one wanted to use it as a vfs cache directory)", path.string() ) );
}

END_VFS_NAMESPACE
