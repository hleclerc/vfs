#pragma once

#include "vfs_namespace.h"

BEG_VFS_NAMESPACE

/*
*/
class OnInit {
public:
    using       Func   = void();

    /**/        OnInit ( Func *f );

    static void update ();

    OnInit*     prev;  ///<
    Func*       func;  ///<
};

#define ON_INIT static void _on_init_func(); static VFS_NAMESPACE::OnInit _on_init_obj( _on_init_func ); static void _on_init_func()

END_VFS_NAMESPACE
