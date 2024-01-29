#include "../support/string/va_string.h"
#include "../support/string/read_file.h"
#include "../support/string/ctor_for.h"
#include "../support/string/join.h"

#include "../support/lexical_comparison.h"
#include "../support/push_back_unique.h"
#include "../support/used_sources.h"
#include "../support/check_dir.h"
#include "../support/OnInit.h"

#include "../rapidjson/document.h"

#include "VfsSymbolCache.h"

#include <dlfcn.h>
#include <fstream>

BEG_VFS_NAMESPACE

std::filesystem::path vfs_symbol_source_directory = ".vfs_symbol_cache";
VfsSymbolCache vfs_symbol_cache;

Vec<Str> VfsSymbolCache::global_cpp_flags;

VfsSymbolCache::VfsSymbolCache() {
}

void VfsSymbolCache::register_func( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const Vec<Vec<Str>> &ct_casts, const CompilationFlags &cn, void *symbol ) {
    Key key{ name, return_type, arg_types, ct_casts, cn };
    loaded_symbols[ key ] = symbol;
}

VfsSymbolCache::SurdefFunc &VfsSymbolCache::add_surdef( const Str &file, int line, const std::regex &name, const Vec<Str> &arg_names ) {
    PI size = surdefs.size();
    surdefs.push_back( Surdef{ file, line, name, arg_names } );
    return surdefs[ size ].f;
}

void *VfsSymbolCache::find_func( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const Vec<Vec<Str>> &ct_casts, const CompilationFlags &cn ) {
    Key key{ name, return_type, arg_types, ct_casts, cn };
    auto iter = loaded_symbols.find( key );
    if ( iter == loaded_symbols.end() )
        iter = loaded_symbols.insert( iter, { key, load_lib_for( name, return_type, arg_types, ct_casts, cn ) } );
    return iter->second;
}

Str VfsSymbolCache::make_tmp_file( PI64 base ) {
    check_dir( vfs_symbol_source_directory / "tmp" );
    for( ; ++base; ) {
        std::filesystem::path file = vfs_symbol_source_directory / "tmp" / std::to_string( base );
        if ( ! std::filesystem::exists( file ) ) {
            std::ofstream fout( file );
            if ( ! fout )
                ERROR( va_string( "impossible to create file $0", file.string() ) );
            fout.close();
            return file.string();
        }
    }
    return {};
}

void *VfsSymbolCache::load_lib_for( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const Vec<Vec<Str>> &ct_casts, const CompilationFlags &cn ) {
    // get surdef, make the corresponding cpp
    Str cpp_content = cpp_for( name, return_type, arg_types, ct_casts, cn );

    // find file with the right line or make it
    std::hash<Str> hash;
    check_build_config_file();
    check_dir( vfs_symbol_source_directory );
    for( PI64 h = hash( cpp_content ); ; ++h ) {
        // find a place to write the .cpp file (the same name may be used for another content)
        Str base_name = name + "_" + std::to_string( h );
        Path cpp_filename = vfs_symbol_source_directory / ( base_name + ".cpp" );
        Opt<Str> cur_content = read_file( cpp_filename.string() );
        if ( ! cur_content || *cur_content == cpp_content ) {
            if ( ! cur_content ) {
                std::ofstream fout( cpp_filename );
                fout << cpp_content;
            }
            
            Vec<Str> missing_cpp_files{ cpp_filename.string() };
            while( ! missing_cpp_files.empty() ) {
                Str cpp_to_compile = missing_cpp_files.pop_back_val();

                // launch vfs_build with output_info in a tmp file
                Str output_info_name = make_tmp_file( h );
                Vec<Str> args{ "vfs_build", "lib", cpp_to_compile,
                    "--write-output-info-to=" + output_info_name,
                    "--do-not-link-deps" //, "-v10"
                };
                for( const Str &flag : global_cpp_flags )
                    push_back_unique( args, flag );

                run( args );

                // read output info
                Str output_info = *ASSERTED( read_file( output_info_name ) );
                std::filesystem::remove( output_info_name );
                output_info = *ASSERTED( read_file( output_info ) );

                rapidjson::Document json;
                json.Parse( output_info.c_str() );
                auto output_kwargs = json[ "output_kwargs" ].GetObject();

                Str out_name = output_kwargs[ "out_name" ].GetString();
                for( const auto &cpp : json[ "output_kwargs" ][ "cpp_deps" ].GetArray() ) {
                    Str cpp_file = cpp.GetString();
                    if ( used_sources.insert( cpp_file ).second )
                        missing_cpp_files.push_back( cpp_file );
                }

                // load lib
                load_lib( out_name );
            }


            // look again in loaded_symbols
            Key key{ name, return_type, arg_types, ct_casts, cn };
            auto iter = loaded_symbols.find( key );
            if ( iter == loaded_symbols.end() )
                ERROR( va_string( "library did not add the symbol for key $0", key ) );
            return iter->second;
        }
    }
}

Str VfsSymbolCache::cpp_for( const Str &function_name, const Str &return_type, const Vec<Str> &arg_types, const Vec<Vec<Str>> &ct_casts, const CompilationFlags &cn ) {
    //
    struct SurdefTrial { Str file; int line; VfsCodegen cg; VfsSurdefStage vss; };
    auto try_surdef = [&]( const Surdef &surdef ) -> SurdefTrial {
        VfsCodegen cg;
        cg.add_cpp_flag( "-g3");
        cg.add_cpp_flag( "-I" VFS_SOURCE_DIR );
        cg.add_inc_file( "vfs/vfs_system/VfsSymbolCache.h" );
        cg.add_preliminary( "USING_VFS_NAMESPACE" );
        cg.function_name = function_name;
        cg.return_type = return_type;
        cg.arg_types = arg_types;

        cn.add_flags_to( cg );

        // arg_names
        Vec<Str> func_arg_names;
        Vec<Str> func_arg_decls;
        for( PI i = 0, n = 0; i < arg_types.size(); n += ct_casts[ i++ ].size() ) {
            Str func_arg_name;
            if ( ct_casts[ i ].size() == 1 ) {
                func_arg_name = ( i == n && n < surdef.arg_names.size() ? surdef.arg_names[ n ] : ( "arg_" + std::to_string( n ) ) );
                if ( ct_casts[ i ][ 0 ].size() )
                    func_arg_name += "_decl";
            } else
                func_arg_name = "__va_arg_" + std::to_string( i );
            func_arg_names.push_back( func_arg_name );

            bool nosp = arg_types[ i ].ends_with( "&" ) || arg_types[ i ].ends_with( "*" );
            func_arg_decls.push_back( arg_types[ i ] + ( nosp ? ""  : " " ) + func_arg_name );
        }

        // opening
        cg.add_line( "static $0 __func( $1 ) {", cg.return_type, join( func_arg_decls, ", " ) );

        // casts
        for( PI i = 0, n = 0; i < arg_types.size(); ++i ) {
            for( PI j = 0; j < ct_casts[ i ].size(); ++j, ++n ) {
                Str cast = ct_casts[ i ][ j ];
                if ( cast.empty() && i != n )
                    cast = "auto &&{ARG} = FORWARD( {ARG_DECL} );";

                Str cg_arg_name = n < surdef.arg_names.size() ? surdef.arg_names[ n ] : ( "arg_" + std::to_string( n ) );
                cast = std::regex_replace( cast, std::regex( "\\{ARG_DECL\\}" ), func_arg_names[ i ] );
                cast = std::regex_replace( cast, std::regex( "\\{ARG\\}" ), cg_arg_name );
                cg.arg_names.push_back( cg_arg_name );
                cg.arg_casts.push_back( cast );
                if ( cast.size() )
                    cg.add_line( cast );
            }
        }

        // body
        VfsSurdefStage vss = surdef.f( cg );

        // closing
        cg.add_line( "}" );

        cg.add_init_line( "vfs_symbol_cache.register_func( $0, $1, $2, $3, $4, (void *)&__func );", ctor_for( function_name ), ctor_for( return_type ), ctor_for( arg_types ), ctor_for( ct_casts ), ctor_for( cn ) );

        return { surdef.file, surdef.line, std::move( cg ), vss };
    };

    // find surdef
    Vec<SurdefTrial> surdef_trials;
    Vec<double> best_pertinence;
    for( const Surdef &surdef : surdefs ) {
        if ( ! std::regex_search( function_name, surdef.name ) )
            continue;

        // temporary (should not appear)
        bool has_same_file_and_line = false;
        for( const SurdefTrial &surdef_trial : surdef_trials )
            if ( surdef_trial.file == surdef.file && surdef_trial.line == surdef.line )
                has_same_file_and_line = true;
        if ( has_same_file_and_line )
            continue;

        SurdefTrial surdef_trial = try_surdef( surdef );
        if ( surdef_trial.vss.result == VfsSurdefStage::Result::invalid )
            continue;

        if ( lexical_comparison( surdef_trial.vss.pertinence, best_pertinence ) )
            continue;

        if ( std::ranges::lexicographical_compare( best_pertinence, surdef_trial.vss.pertinence ) ) {
            best_pertinence = surdef_trial.vss.pertinence;
            surdef_trials.clear();
        }
        surdef_trials.push_back( std::move( surdef_trial ) );

        if ( surdef_trial.vss.result == VfsSurdefStage::Result::needs_a_second_pass )
            TODO;
    }

    if ( surdef_trials.empty() )
        ERROR( "found no match..." );

    if ( surdef_trials.size() >= 2 ) {
        for( const SurdefTrial &st : surdef_trials )
            std::cerr << st.file << ":" << st.line << ": error: possible surdef" << std::endl;
        ERROR( "ambiguous overload..." );
    }

    //
    // compile( surdef_trials[ 0 ].file );
    std::ostringstream ss;
    surdef_trials[ 0 ].cg.write_cpp_to( ss );
    return ss.str();
}

int VfsSymbolCache::run( const Vec<Str> &cmd_vec ) {
    // P( cmd );
    Str cmd = join( cmd_vec, " " );
    if ( int res = system( cmd.c_str() ) ) {
        // std::cerr << "while running '" << cmd << "'" << std::endl;
        exit( res );
    }
    return 0;
}

void VfsSymbolCache::check_build_config_file() {
    Path config = vfs_symbol_source_directory / "vfs_build_config.py";
    if ( ! std::filesystem::exists( config ) ) {
        std::ofstream fout( config );
        fout << "import vfs\n";
        fout << "def config( options ):\n";
        fout << "    vfs.vfs_build_config( options )\n";
    }
}


void VfsSymbolCache::load_lib( const std::filesystem::path &so_filename ) {
    void *dl = dlopen( so_filename.c_str(), RTLD_NOW | RTLD_GLOBAL );
    if ( ! dl ) {
        std::cerr << "while dlopening " << dlerror() << std::endl;
        assert( 0 );
    }

    libs.push_back( dl );

    OnInit::update();
}

END_VFS_NAMESPACE
