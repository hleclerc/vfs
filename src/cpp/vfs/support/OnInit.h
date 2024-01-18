#pragma once

namespace Vfs {

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

#define ON_INIT static void _on_init_func(); static Vfs::OnInit _on_init_obj( _on_init_func ); static void _on_init_func()

}
