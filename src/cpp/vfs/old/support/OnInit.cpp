#include "OnInit.h"

BEG_VFS_NAMESPACE

OnInit *last_OnInit = nullptr;

OnInit::OnInit( Func *func ) : func( func ) {
    if ( func ) {
        prev = last_OnInit;
        last_OnInit = this;
    }
}

void OnInit::update() {
    while ( OnInit *loi = last_OnInit ) {
        last_OnInit = nullptr;
        for( ; loi; loi = loi->prev )
            loi->func();
    }
}

END_VFS_NAMESPACE
