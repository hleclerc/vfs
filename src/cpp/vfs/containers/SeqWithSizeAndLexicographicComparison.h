#pragma once

#include "Seq.h"

BEG_VFS_NAMESPACE

template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
struct SeqWithSizeAndLexicographicComparison : Seq<Item,static_size,local_size,alignment,allow_heap> {
    using Seq<Item,static_size,local_size,alignment,allow_heap>::Seq;

    bool operator<( const auto &that ) { return size_and_lexical_comparison( *this, that ); }
};

END_VFS_NAMESPACE
