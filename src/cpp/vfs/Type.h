#pragma once

#include "vfs_system/ct_value.h"

BEG_VFS_NAMESPACE

///
class Type { VFS_TD_ATTRIBUTES( Type, "vfs" );
public:
    /**/ Type( auto &&name, auto &&compilation_flags, auto &&...template_parameters ); ///< template_parameters are automatically transformed to ct_value to be used as real template args
    /**/ Type( auto name ) : Type( name, CompilationFlags() ) {}
    TT   Type( CtType<T> ) : Type( FromValue(), CtType<CtType<T>>() ) {}
    /**/ Type() : Type( CtType<Void>() ) {}
};

// to get the Type of a VfsObject
auto type_of( const HasVfsTd auto &v ) { return VFS_CALL( type_of, Type, v ); }

inline const auto &ct_value( const Type &v ) { return v; }

// impl ----------------------------------------------------------------------------------------------------------------------------------
Type::Type( auto &&name, auto &&compilation_flags, auto &&...template_parameters ) {
    VFS_CALL_DINK( construct_Type, void, _vfs_type_and_data, ct_value( name ), ct_value( compilation_flags ), ct_value( template_parameters )... );
}

END_VFS_NAMESPACE
