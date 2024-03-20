#pragma once

#include "../support/TypeConfig.h"

BEG_VFS_NAMESPACE

/// ct known size version
template<class T,int size_=-1>
struct Span {
    constexpr          Span      ( T *data ) : data_( data ) {}

    constexpr auto     size      () const { return CtInt<size_>(); }

    constexpr const T& operator[]( PI index ) const { return data_[ index ]; }
    constexpr T&       operator[]( PI index ) { return data_[ index ]; }

    const T*           begin     () const { return data_; }
    T*                 begin     () { return data_; }

    const T*           end       () const { return data_ + size_; }
    T*                 end       () { return data_ + size_; }

    T*                 data_;
};

/// dynamic version
template<class T>
struct Span<T,-1> {
    constexpr          Span      ( T *data, PI size ) : data_( data ), size_( size ) {}

    constexpr auto     size      () const { return size_; }

    constexpr const T& operator[]( PI index ) const { return data_[ index ]; }
    constexpr T&       operator[]( PI index ) { return data_[ index ]; }

    const T*           begin     () const { return data_; }
    T*                 begin     () { return data_; }

    const T*           end       () const { return data_ + size_; }
    T*                 end       () { return data_ + size_; }

    T*                 data_;
    PI                 size_;
};

// common functions
#define DTP template<class T,int s>
#define UTP Span<T,s>

DTP auto get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/containers/Span.h" ); }
DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<T>() ); f( CtInt<s>() ); }
DTP auto template_type_name( CtType<UTP> ) { return "Vec"; }

#undef DTP
#undef UTP

END_VFS_NAMESPACE
