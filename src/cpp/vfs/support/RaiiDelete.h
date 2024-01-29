#pragma once

BEG_VFS_NAMESPACE

template<class T>
struct RaiiDelete {
    /**/  RaiiDelete( T *ptr ) : ptr( ptr ) {}
    /**/ ~RaiiDelete() { delete ptr; }

    T*    ptr;
};

template<class T>
RaiiDelete<T> raii_delete( T *val ) {
    return val;
}

END_VFS_NAMESPACE
