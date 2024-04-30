#pragma once

#include "VfsTdKey.h"

BEG_VFS_NAMESPACE
/// used to select the right VfsTdImpl
template<class Wrapper,class Data> struct alignas( Wrapper ) VfsTdImplTest { VfsTdKey<Wrapper> key; Data data; };

/// declaration of a proxy/pointer to symbolic or real reference
template<class Wrapper,class Impl,class Data,bool as_a_value=( sizeof( VfsTdImplTest<Wrapper,Data> ) <= sizeof( Wrapper ) )>
class VfsTdImpl;

// inline data
template<class Wrapper_,class Impl,class Data>
class alignas( Wrapper_ ) VfsTdImpl<Wrapper_,Impl,Data,true> {
public:
    using             Wrapper      = Wrapper_;

    /**/              VfsTdImpl    ( auto &&...args ) : key( CtType<Impl>() ), data( FORWARD( args )... ) {}

    const auto&       wrapped_value() const { return data; }
    auto&             wrapped_value() { return data; }

    VfsTdKey<Wrapper> key;      ///<
    Data              data;     ///<
};

// data on the heap
template<class Wrapper_,class Impl,class Data>
class alignas( Wrapper_ ) VfsTdImpl<Wrapper_,Impl,Data,false> {
public:
    using             Wrapper      = Wrapper_;
    using             Ptr          = std::unique_ptr<Data>;

    /**/              VfsTdImpl    ( auto &&...args ) : key( CtType<Impl>() ), ptr( new Data( FORWARD( args )... ) ) {}

    const auto&       wrapped_value() const { return *ptr; }
    auto&             wrapped_value() { return *ptr; }

    VfsTdKey<Wrapper> key;         ///<
    Ptr               ptr;         ///<
};

END_VFS_NAMESPACE

