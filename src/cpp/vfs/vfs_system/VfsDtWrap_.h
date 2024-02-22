#pragma once

#include "VfsDtType.h"
#include <memory>

BEG_VFS_NAMESPACE

/// What VfsDt objects actually store
template<class Object,class Content>
struct VfsDtWrap_ {
    struct alignas( Object::VfsDtSpec::alig ) DataDirect {
        /**/                     DataDirect( auto &&...ctor_args ) : content( FORWARD( ctor_args )... ) {}
        auto*                    ptr       () const { return &content; }

        Content                  content;  ///<
    };

    struct alignas( Object::VfsDtSpec::alig ) DataHeap {
        /**/                     DataHeap  ( auto &&...ctor_args ) : content( std::make_unique<Content>( FORWARD( ctor_args )... ) ) {}
        auto*                    ptr       () const { return content.get(); }

        std::unique_ptr<Content> content;  ///<
    };

    using        Data       = std::conditional_t<( alignof( Content ) > Object::VfsDtSpec::alig || sizeof( DataDirect ) > sizeof( typename Object::VfsDtSpec ) ), DataHeap, DataDirect>;

    /**/         VfsDtWrap_( auto &&...args ) : data( FORWARD( args )... ) {
        const auto &type = StaticStorage<VfsDtType<Object,Content,0>>::value;
        instantiated_type_index = type.instantiated_type_index;
        global_type_index = type.global_type_index;
    }

    mutable PI32 instantiated_type_index; ///<
    PI32         global_type_index;       ///<
    Data         data;                    ///<
};

END_VFS_NAMESPACE
