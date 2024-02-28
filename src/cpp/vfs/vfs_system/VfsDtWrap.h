#pragma once

#include "VfsFunc.h" // IWYU pragma: export

BEG_VFS_NAMESPACE
class Type;

/// raw content of VfsDtObjects (like `Scalar`, `Type`, ...)
/// => store type info + some room for inline data (that can be a pointer to the content if data_size is not large enough)
template<class Object_, int size_ = 3 * sizeof( void * ), int alig_ = alignof( void * )>
class alignas( alig_) VfsDtWrap {
public:
    static constexpr int data_size                = ( 2 * sizeof( PI32 ) + alig_ - 1 ) / alig_ * alig_ - 2 * sizeof( PI32 ) + size_;
    using                Object                   = Object_;
    static constexpr int alig                     = alig_;

    TT void              construct                ( FromTypeAndCtorArguments, CtType<T>, auto &&...ctor_args ); ///< with ( ctor_args... ) (and not {})
    TT void              construct                ( FromPointer, T &&pointer );
    TT void              construct                ( FromValue, T &&value );

    static auto          get_compilation_flags    ( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/VfsDt.h" ); }
    static void          for_each_template_arg    ( auto &&f ) { f( CtType<Object>() ); f( CtInt<size_>() ); f( CtInt<alig_>() ); }
    static auto          template_type_name       () { return "VfsDt"; }

    mutable PI32         instantiated_type_index; ///<
    PI32                 global_type_index;       ///<
    char                 data[ data_size ];       ///<
};

END_VFS_NAMESPACE

#include "VfsDtWrap.tcc" // IWYU pragma: export
