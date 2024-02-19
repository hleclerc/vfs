#pragma once

#include "../support/TypeConfig.h"
#include <memory>

BEG_VFS_NAMESPACE

/// How VfsArgWithRtIntType content is actually stored
template<class Arg,class Content>
struct CtDataOfVfsArgWithRtIntType {
    struct alignas( Arg::alig ) DataDirect {

        auto*                    ptr() const { return &content; }

        mutable PI32             instantiated_type_index; ///<
        PI32                     global_type_index;       ///<
        Content                  content;                 ///<
    };

    struct alignas( Arg::alig ) DataHeap {
        auto*                    ptr() const { return content.get(); }

        mutable PI32             instantiated_type_index; ///<
        PI32                     global_type_index;       ///<
        std::unique_ptr<Content> content;                 ///<
    };

    using       Data                       = std::conditional_t<( alignof( Content ) > Arg::alig || sizeof( DataDirect ) > sizeof( Arg ) ), DataHeap, DataDirect>;

    /**/        CtDataOfVfsArgWithRtIntType( auto &&...args ) : data( FORWARD( args )... ) {}

    Data        data;                      ///<
};

END_VFS_NAMESPACE
