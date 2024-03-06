#pragma once

#include "VfsDtType.h"
#include <memory>

BEG_VFS_NAMESPACE

/// What VfsDt objects actually store
template<class Object,class Content,class RefAccess>
struct VfsDtWrap {
    /// inline storage
    struct alignas( Object ) DataDirect {
        /**/                     DataDirect( auto &&...ctor_args ) : content( FORWARD( ctor_args )... ) {}
        auto*                    ptr       () const { return &content; }

        mutable PI32             instantiated_type_index; ///<
        PI32                     global_type_index;       ///<
        Content                  content;                 ///<
    };

    /// indirect storage
    struct alignas( Object ) DataHeap {
        /**/                     DataHeap  ( auto &&...ctor_args ) : content( std::make_unique<Content>( FORWARD( ctor_args )... ) ) {}
        auto*                    ptr       () const { return content.get(); }

        mutable PI32             instantiated_type_index; ///<
        PI32                     global_type_index;       ///<
        std::unique_ptr<Content> content;                 ///<
    };

    using        Data      = std::conditional_t< ( alignof( DataDirect ) > alignof( Object ) || sizeof( DataDirect ) > sizeof( Object ) ), DataHeap, DataDirect >;

    /**/         VfsDtWrap( auto &&...args ) : data( FORWARD( args )... ) {
        const auto &type = StaticStorage<VfsDtType<Object,Content,RefAccess>>::value;
        data.instantiated_type_index = type.instantiated_type_index;
        data.global_type_index = type.global_type_index;
    }

    void         destroy() { data.~Data(); }

    Data         data;    ///<
};

END_VFS_NAMESPACE
