#pragma once

#include "vfs_system/VfsDtObject.h"
#include "vfs_system/ct_value.h" // IWYU pragma: export
#include "support/ct_known.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

///
class Type {
public:
    VfsDtObject_STD_METHODS( Type, "vfs", sizeof( void * ), alignof( void * ) );

    /**/ Type( auto &&name, auto &&compilation_flags, auto &&...template_parameters ); ///< template_parameters are automatically transformed to ct_value to be used as real template args
    /**/ Type( auto name ) : Type( name, CompilationFlags() ) {}
    TT   Type( CtType<T> ) : Type( FromValue(), CtType<CtType<T>>() ) {}
    /**/ Type() : Type( CtType<Void>() ) {}
};

// to get the Type of a VfsObject
auto actual_type_of( const VfsArg auto &v ) { return VFS_CALL( actual_type_of, CtStringList<>, Type, v ); }

inline const auto &ct_value( const Type &v ) { return v; }

// impl ----------------------------------------------------------------------------------------------------------------------------------
Type::Type( auto &&name, auto &&compilation_flags, auto &&...template_parameters ) {
    VFS_CALL( construct_Type, CtStringList<>, void, __vfs_dt_attributes, ct_value( name ), ct_value( compilation_flags ), ct_value( template_parameters )... );
}

///
#define MAKE_TYPE( NAME, INCLUDE, ... ) \
    ( [&]( auto&&...args ) { \
        if constexpr (( requires { ct_known( args ); } && ... )) { \
            return CtType<NAME<VALUE_IN_DECAYED_TYPE_OF( args )...>>(); \
        } else { \
            return Type( #NAME, "inc_file:" INCLUDE, FORWARD( args )... ); \
        } \
    } )( __VA_ARGS__ )

END_VFS_NAMESPACE
