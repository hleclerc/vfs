#pragma once

#include "VfsTdKey.h"

BEG_VFS_NAMESPACE
/// used to select the right VfsTdImpl
template<class Wrapper,class Data> struct alignas( Wrapper ) VfsTdImplTest { VfsTdKey<Wrapper> key; Data data; };

/// declaration of a proxy/pointer to symbolic or real reference
template<class Wrapper,class Impl,class Data,bool as_a_value=( sizeof( VfsTdImplTest<Wrapper,Data> ) <= sizeof( Impl ) )>
class VfsTdImpl;

// inline data
template<class Wrapper_,class Impl,class Data>
class alignas( Wrapper_ ) VfsTdImpl<Wrapper_,Impl,Data,true> {
public:
    using             Wrapper   = Wrapper_;

    /**/              VfsTdImpl ( const VfsTdImpl &that ) : key( that.key ), data( that.data ) {}
    /**/              VfsTdImpl ( VfsTdImpl &&that ) : key( std::move( that.key ) ), data( std::move( that.data ) ) {}
    /**/              VfsTdImpl ( auto &&...args ) : key( CtType<Impl>() ), data( FORWARD( args )... ) {}

    const auto&       lvalue    () const { return data; }
    auto&&            rvalue    () { return std::move( data ); }

    VfsTdKey<Wrapper> key;      ///<
    Data              data;     ///<
};

// data on the heap
template<class Wrapper_,class Impl,class Data>
class alignas( Wrapper_ ) VfsTdImpl<Wrapper_,Impl,Data,false> {
public:
    using             Wrapper   = Wrapper_;
    using             Ptr       = std::unique_ptr<Data>;

    /**/              VfsTdImpl ( const VfsTdImpl &that ) : key( that.key ), ptr( new Data( *that.ptr ) ) {}
    /**/              VfsTdImpl ( VfsTdImpl &&that ) : key( std::move( that.key ) ), ptr( std::move( that.ptr ) ) {}
    /**/              VfsTdImpl ( auto &&...args ) : key( CtType<Impl>() ), ptr( new Data( FORWARD( args )... ) ) {}

    const auto&       lvalue    () const { return *ptr; }
    auto&&            rvalue    () { return std::move( *ptr ); }

    VfsTdKey<Wrapper> key;      ///<
    Ptr               ptr;      ///<
};

END_VFS_NAMESPACE

