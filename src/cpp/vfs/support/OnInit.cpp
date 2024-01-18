#include "OnInit.h"

namespace Vfs {

OnInit *last_OnInit = nullptr;

OnInit::OnInit( Func *func ) : func( func ) {
    if ( func ) {
        prev = last_OnInit;
        last_OnInit = this;
    }
}

void OnInit::update() {
    for( ; last_OnInit; last_OnInit = last_OnInit->prev )
        last_OnInit->func();
}

}
