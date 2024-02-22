#pragma once

#include "VfsFunc.h" // IWYU pragma: export

BEG_VFS_NAMESPACE
class Type;

/// raw content of `VfsDtObject`s (like `Scalar`, `Type`, ...)
/// VfsArg with Dynamic Type and some room for inline data (that can be the content or a pointer to the content)
template<class Object_, int size_ = 3 * sizeof( void * ), int alig_ = alignof( void * )>
class alignas( alig_) VfsDtRtData {
public:
    static constexpr int data_size                = ( 2 * sizeof( PI32 ) + alig_ - 1 ) / alig_ * alig_ - 2 * sizeof( PI32 ) + size_;
    using                Object                   = Object_;
    static constexpr int alig                     = alig_;

    TT void              construct                ( FromTypeAndCtorArguments, CtType<T>, auto &&...ctor_args ); ///< with ( ctor_args... ) (and not {})
    TT void              construct                ( FromPointer, T &&pointer );
    TT void              construct                ( FromValue, T &&value );

    TT void              destroy                  ( CtType<T> );

    // TT constexpr bool not_enough_room_for      ( CtType<T> );
    // void* const&      void_ptr                 () const;
    // void*&            void_ptr                 ();
    // TT const T&       cast                     ( CtType<T> ) const;
    // TT T&             cast                     ( CtType<T> );
    // TT const T&       cast                     () const;
    // TT T&             cast                     ();

    static auto          get_compilation_flags    ( CompilationFlags &cn ) { cn.add_inc_file( "vfs/vfs_system/VfsDt.h" ); }
    static void          for_each_template_arg    ( auto &&f ) { f( CtType<Object>() ); f( CtInt<size_>() ); f( CtInt<alig_>() ); }
    static auto          template_type_name       () { return "VfsDt"; }

    mutable PI32         instantiated_type_index; ///<
    PI32                 global_type_index;       ///<
    char                 data[ data_size ];       ///<
};

END_VFS_NAMESPACE

#include "VfsDtRtData.tcc" // IWYU pragma: export
