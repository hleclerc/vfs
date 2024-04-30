#pragma once

#include "../support/on_wrapped_value.h"
#include "../support/reassign.h"
#include "VfsTdImpl.h"

BEG_VFS_NAMESPACE

///
template<class Wrapper,class Impl,class Data>
struct VfsTdImplWithBaseMethods : VfsTdImpl<Wrapper,Impl,Data> {
    /**/         VfsTdImplWithBaseMethods( auto &&...args ) : VfsTdImpl<Wrapper,Impl,Data>( FORWARD( args )... ) {}

    DisplayItem* display                ( auto &ds ) const { return VFS_NAMESPACE::display( ds, this->wrapped_value() ); }

    /// pmt = potentially modifiable type
    auto         reassign_pmt           ( auto &&cb, auto &&that ) {
        // if that has a wrapped value
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
};


END_VFS_NAMESPACE
