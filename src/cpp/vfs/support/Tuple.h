#pragma once

#include "vfs_namespace.h"

BEG_VFS_NAMESPACE

template<class... Items>
struct Tuple;


template<class Head,class... Tail>
struct Tuple<Head,Tail...> {
    using Next = Tuple<Tail...>;

    Head  head;
    Next  next;
};

template<>
struct Tuple<> {
};

END_VFS_NAMESPACE
