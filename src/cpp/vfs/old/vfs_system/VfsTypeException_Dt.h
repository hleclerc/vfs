#pragma once

#include "../support/type_name.h"
#include "VfsTypeException.h"

BEG_VFS_NAMESPACE

template<class Object,class NewType,class OldType>
struct VfsTypeException_Dt : VfsTypeException {
    VfsTypeException_Dt( OldType &&src ) : src( std::move( src ) ) {
    }

    virtual void change_type_of( void *object_ ) const {
        auto &object = *reinterpret_cast<Object *>( object_ );
        object.~Object();

        P( "update type", type_name<NewType>(), type_name<OldType>() );
        new ( &object ) Object( FromTypeAndCtorArguments(), CtType<NewType>(), std::move( src ) );
    }

    OldType src;
};

END_VFS_NAMESPACE
