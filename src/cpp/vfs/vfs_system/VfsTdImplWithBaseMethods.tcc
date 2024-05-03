#pragma once

#include "VfsTdImplWithBaseMethods.h"
#include "../support/on_wrapped_value.h"
#include "../support/reassign.h"

#define DTP template<class Wrapper,class Impl,class Data>
#define UTP VfsTdImplWithBaseMethods<Wrapper,Impl,Data>

BEG_VFS_NAMESPACE

DTP UTP::VfsTdImplWithBaseMethods( auto &&...args ) : VfsTdImpl<Wrapper,Impl,Data>( FORWARD( args )... ) {
}

DTP DisplayItem* UTP::display( auto &ds ) const {
    return VFS_NAMESPACE::display( ds, this->wrapped_value() );
}

/// pmt = potentially modifiable type
DTP auto UTP::reassign_pmt( auto &&cb, auto &&that ) {
    // if `that` is a wrapped value => call with the wrapped value
    if constexpr ( requires { on_wrapped_value( FORWARD( that ), []( auto && ) {} ); } )
        return on_wrapped_value( FORWARD( that ), [&]( auto &&value ) { return reassign_pmt( FORWARD( cb ), FORWARD( value ) ); } );
    else

    // call `reassign`
    if constexpr ( requires { reassign( this->wrapped_value(), FORWARD( that ) ); } ) {
        if ( reassign( this->wrapped_value(), FORWARD( that ) ) )
            return cb( *this );
        else {
            using St = STORAGE_TYPE_OF( that );
            return cb( *(new ( this ) Impl::template WithData<St>::value( FORWARD( that ) ) ) );
        }
    }

    // else, try operator=
    else {
        this->wrapped_value() = FORWARD( that );
        return cb( *this );
    }
}

/// pmt = potentially modifiable type
DTP auto UTP::self_op_pmt( auto &&cb, auto &&functor, auto &&that ) {
    // if `that` is a wrapped value => call with the wrapped value
    if constexpr ( requires { on_wrapped_value( FORWARD( that ), []( auto && ) {} ); } )
        return on_wrapped_value( FORWARD( that ), [&]( auto &&value ) { return self_op_pmt( FORWARD( cb ), FORWARD( functor ), FORWARD( value ) ); } );
    else

    // if impossible to do without changing the type
    if constexpr ( std::is_same_v<decltype( functor( this->wrapped_value(), FORWARD( that ) ) ),PrimitiveCtInt<0>> ) {
        using St = STORAGE_TYPE_OF( functor.op( this->wrapped_value(), FORWARD( that ) ) );
        return cb( *(new ( this ) Impl::template WithData<St>::value( functor.op( this->wrapped_value(), FORWARD( that ) ) ) ) );
    } else

    //
    {
        functor( this->wrapped_value(), FORWARD( that ) );
        return cb( *this );
    }
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
