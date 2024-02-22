#pragma once

#include "../support/size_and_lexical_comparison.h"
#include "../support/CompilationFlags.h"
#include "../support/OnInit.h" // IWYU pragma: export
#include "VfsSurdefStage.h"
#include "VfsCodegen.h"
#include <filesystem>
#include <functional>
#include <regex>
#include <map>

BEG_VFS_NAMESPACE

/// load or make symbols (i.e. function code, values, ...)
class VfsSymbolCache {
public:
    using           Path                   = std::filesystem::path;
    using           Key                    = Tuple<Str, Str, Vec<Str>, CompilationFlags, Vec<Vec<Str>>, Vec<Vec<Str>>, Vec<Str>, Vec<Str>>;
    using           LoadedSymbols          = std::map<Key,void *,SizeAndLexicalComparison>;
    using           SurdefFunc             = std::function<VfsSurdefStage( VfsCodegen &cg )>;

    /**/            VfsSymbolCache         ();
    
    void            register_func          ( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types, const Vec<Str> &cast_refs, void *symbol );
    SurdefFunc&     add_surdef             ( const Str &file, int line, const std::regex &name, const Vec<Str> &arg_names = {} );
    void*           find_func              ( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types, const Vec<Str> &cast_refs );
    
    static Vec<Str> global_cpp_flags;

private:
    struct          Surdef                 { Str file; int line; std::regex name; Vec<Str> arg_names; SurdefFunc f; Str arg_name( PI i ) const; };

    Str             make_tmp_file          ( PI64 base );
    void*           load_lib_for           ( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types, const Vec<Str> &cast_refs );
    Str             cpp_for                ( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types, const Vec<Str> &cast_refs );

    void            check_build_config_file();
    void            load_lib               ( const std::filesystem::path &so_filename );
    int             run                    ( const Vec<Str> &cmd );

    LoadedSymbols   loaded_symbols;
    Vec<Surdef>     surdefs;
    Vec<void*>      libs;
};

extern std::filesystem::path vfs_symbol_source_directory;
extern VfsSymbolCache vfs_symbol_cache;

#define VFS_ADD_SURDEF( REGEX, ... ) \
    vfs_symbol_cache.add_surdef( __FILE__, __LINE__, std::regex( REGEX ), { __VA_ARGS__ } ) = [&]( VfsCodegen &cg ) -> VfsSurdefStage

END_VFS_NAMESPACE
