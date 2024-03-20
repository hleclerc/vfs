#pragma once

#include "../vfs_namespace.h"
#include <string>

BEG_VFS_NAMESPACE

/**
*/
class DisplayParameters {
public:
    using       Str                        = std::string;

    static auto for_debug_info             () { return DisplayParameters{}; }

    bool        force_scientific_precision = false; ///<
    bool        use_spaces_for_readability = true; ///<
    Str         escape_sequence_for_keys   = "1"; ///<
    Str         inline_list_delimiter      = ","; ///<
    int         numerical_precision        = -1; ///<
    int         screen_size                = -1; ///<
    bool        single_line                = true; ///<
    int         tab_size                   = 2; ///< -1 => use the tab character. Else, use spaces
};

END_VFS_NAMESPACE
