#include "../support/string/string_simple_match.h"
#include "../support/string/string_replace.h"
#include "../support/string/va_string.h"
#include "../support/string/read_file.h"
#include "../support/string/ctor_for.h"
#include "../support/string/join.h"

#include "../support/lexical_comparison.h"
#include "../support/push_back_unique.h"
#include "../support/used_sources.h"
#include "../support/used_flags.h"
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

void VfsSymbolCache::register_func( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types, void *symbol ) {
    Key key{ name, return_type, arg_types, compilation_flags, final_types, final_refs, cast_types };
    loaded_symbols[ key ] = symbol;
}

VfsSymbolCache::SurdefFunc &VfsSymbolCache::add_surdef( const Str &file, int line, const Str &name, const Vec<Str> &arg_names ) {
    PI size = surdefs.size();
    surdefs.push_back( Surdef{ file, line, name, arg_names } );
    return surdefs[ size ].f;
}

void *VfsSymbolCache::find_func( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types ) {
    Key key{ name, return_type, arg_types, compilation_flags, final_types, final_refs, cast_types };
    auto iter = loaded_symbols.find( key );
    if ( iter == loaded_symbols.end() )
        iter = loaded_symbols.insert( iter, { key, load_lib_for( name, return_type, arg_types, compilation_flags, final_types, final_refs, cast_types ) } );
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

void *VfsSymbolCache::load_lib_for( const Str &name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types ) {
    // get surdef, make the corresponding cpp
    Str cpp_content = cpp_for( name, return_type, arg_types, compilation_flags, final_types, final_refs, cast_types );

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
            Vec<Str> libs_to_load;
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

                // TODO: analyze the flags (e.g. to find contradictions, incompatibilities, ...)
                for( const auto &flag : used_flags ) {
                    if ( flag.name == "write-used-sources" || flag.name == "write-used-flags" )
                        continue;
                    push_back_unique( args, "--" + flag.name + "=" + flag.value );
                }

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

                // register lib
                libs_to_load << out_name;
            }

            // load libs
            while ( libs_to_load.size() )
                load_lib( libs_to_load.pop_back_val() );

            // look again in loaded_symbols
            Key key{ name, return_type, arg_types, compilation_flags, final_types, final_refs, cast_types };
            auto iter = loaded_symbols.find( key );
            if ( iter == loaded_symbols.end() )
                ERROR( va_string( "library did not add the symbol for key $0", key ) );
            return iter->second;
        }
    }
}

Str VfsSymbolCache::cpp_for( const Str &function_name, const Str &return_type, const Vec<Str> &arg_types, const CompilationFlags &compilation_flags, const Vec<Vec<Str>> &final_types, const Vec<Vec<Str>> &final_refs, const Vec<Str> &cast_types ) {
    //
    struct SurdefTrial { Str file; int line; VfsCodegen cg; VfsSurdefStage vss; };
    auto try_surdef = [&]( const Surdef &surdef ) -> SurdefTrial {
        VfsCodegen cg;
        cg.add_cpp_flag( "-I" VFS_SOURCE_DIR );
        cg.add_inc_file( "vfs/vfs_system/VfsSymbolCache.h" );
        cg.add_preliminary( "USING_VFS_NAMESPACE" );
        cg.func_name = function_name;
        cg.return_type = return_type;

        for( const auto &fr : final_refs )
            cg.nb_final_refs_from_cast << fr.size();
        cg.cast_types = cast_types;
        cg.arg_types = arg_types;

        compilation_flags.add_flags_to( cg );

        // names + arg_decls
        Vec<Str> arg_decls;
        for( PI i = 0, n = 0; i < arg_types.size(); ++i ) {
            // base arg_name
            Vec<Str> loc_arg_names;
            for( PI j = 0; j < final_refs[ i ].size(); ++j )
                loc_arg_names << surdef.arg_name( n++ );
            Str base_name = join( loc_arg_names, "_", "_and_" );
            Str arg_name = base_name + "_decl";

            // info in cg
            for( PI j = 0; j < loc_arg_names.size(); ++j ) {
                cg.final_types << final_types[ i ][ j ];
                cg.final_names << loc_arg_names[ j ];
            }
            cg.cast_names << base_name + "_cast";
            cg.arg_names << arg_name;

            // args in func decl
            bool nosp = arg_types[ i ].ends_with( "&" ) || arg_types[ i ].ends_with( "*" );
            arg_decls << arg_types[ i ] + ( nosp ? ""  : " " ) + arg_name;
        }

        // opening
        cg.add_line( "static $0 __func( $1 ) {", cg.return_type, join( arg_decls, ", " ) );

        // casts
        for( PI i = 0; i < cast_types.size(); ++i ) {
            const bool is_rvalue = cg.arg_types[ i ].ends_with( "&&" );
            const bool is_const = cg.arg_types[ i ].starts_with( "const " );

            if ( cast_types[ i ].size() )
                cg.add_line( "auto &&$0 = reinterpret_cast<$1$2 $3>( $4 );",
                    cg.cast_names[ i ],
                    is_const ? "const " : "",
                    cast_types[ i ],
                    is_rvalue ? "&&" : "&",
                    cg.arg_names[ i ]
                );
            else
                cg.add_line( "auto &&$0 = $1;", cg.cast_names[ i ], cg.arg_names[ i ] );
        }

        // finals
        for( PI i = 0, n = 0; i < final_refs.size(); ++i ) {
            const bool is_rvalue = cg.arg_types[ i ].ends_with( "&&" );
            const bool is_const = cg.arg_types[ i ].starts_with( "const " );

            for( PI j = 0; j < final_refs[ i ].size(); ++j, ++n ) {
                Str final_name = cg.final_names[ n ];

                // final_refs
                Str ref = final_refs[ i ][ j ];
                ref = string_replace( ref, "{BEG_ARG_FORWARD}", is_rvalue ? "std::move( " : "" );
                ref = string_replace( ref, "{END_ARG_FORWARD}", is_rvalue ? " )" : "" );
                ref = string_replace( ref, "{ARG_CSTNESS}", is_const ? "const " : "" );
                ref = string_replace( ref, "{ARG_REFNESS}", is_rvalue ? "&&" : "&" );
                ref = string_replace( ref, "{FINAL_NAME}", final_name );
                ref = string_replace( ref, "{CAST_NAME}", cg.cast_names[ i ] );
                ref = string_replace( ref, "{CAST_TYPE}", cg.cast_types[ i ] );
                ref = string_replace( ref, "{ARG_NAME}", cg.arg_names[ i ] );

                cg.add_line( "auto &&$0 = $1;", final_name, ref.size() ? ref : cg.cast_names[ i ] );
            }
        }

        // body
        VfsSurdefStage vss = surdef.f( cg );

        // closing
        cg.add_line( "}" );

        cg.add_init_line( "vfs_symbol_cache.register_func( $0, $1, $2, $3, $4, $5, $6, (void *)&__func );",
                ctor_for( function_name ), ctor_for( return_type ), ctor_for( arg_types ), ctor_for( compilation_flags ),
                ctor_for( final_types ), ctor_for( final_refs ), ctor_for( cast_types ) );

        return { surdef.file, surdef.line, std::move( cg ), vss };
    };

    // find surdef
    Vec<SurdefTrial> surdef_trials;
    Vec<double> best_pertinence{ std::numeric_limits<double>::lowest() };
    P( function_name, "-------------------------" );
    for( const Surdef &surdef : surdefs ) {
        P( surdef.name );
        if ( ! string_simple_match( function_name, surdef.name ) )
            continue;

        SurdefTrial surdef_trial = try_surdef( surdef );
        if ( surdef_trial.vss.result == VfsSurdefStage::Result::invalid )
            continue;

        if ( lexical_comparison( surdef_trial.vss.pertinence, best_pertinence, 0.0 ) )
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
        ERROR( va_string( "found no match for function $0", function_name ) );

    if ( surdef_trials.size() >= 2 ) {
        for( const SurdefTrial &st : surdef_trials )
            std::cerr << st.file << ":" << st.line << ": error: possible surdef" << std::endl;
        ERROR( va_string( "ambiguous overload for function $0", function_name ) );
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

Str VfsSymbolCache::Surdef::arg_name( PI i ) const {
    if ( i < arg_names.size() )
        return arg_names[ i ];
    return va_string( "__arg_$0", i );
}

END_VFS_NAMESPACE

