#pragma once

#include "operators/add.h"
#include "operators/sub.h"
#include "operators/mul.h"
#include "operators/div.h"
#include "operators/mod.h"

#include "operators/inf.h"
#include "operators/sup.h"
#include "operators/leq.h"
#include "operators/geq.h"
#include "operators/equ.h"
#include "operators/neq.h"

#include "operators/neg.h"

BEG_VFS_NAMESPACE

/// base class that permits to add operators like `operator+` as friend function that call functions like `add`, `mul`, ...
struct WithDefaultOperators {
    #define ADD_BIN_OP( OP, FUN ) friend constexpr auto OP( auto &&a,  auto &&b ) requires  \
        std::is_base_of_v<WithDefaultOperators,DECAYED_TYPE_OF(a)> || \
        std::is_base_of_v<WithDefaultOperators,DECAYED_TYPE_OF(b)> { return FUN( FORWARD( a ), FORWARD( b ) ); }
    #define ADD_UNA_OP( OP, FUN ) friend constexpr auto OP( auto &&a ) requires  \
        std::is_base_of_v<WithDefaultOperators,DECAYED_TYPE_OF(a)> { return FUN( FORWARD( a ) ); }

    ADD_BIN_OP( operator+ , add )
    ADD_BIN_OP( operator- , sub )
    ADD_BIN_OP( operator* , mul )
    ADD_BIN_OP( operator/ , div )
    ADD_BIN_OP( operator% , mod )
    ADD_BIN_OP( operator< , inf )
    ADD_BIN_OP( operator> , sup )
    ADD_BIN_OP( operator<=, leq )
    ADD_BIN_OP( operator>=, geq )
    ADD_BIN_OP( operator==, equ )
    ADD_BIN_OP( operator!=, neq )

    ADD_UNA_OP( operator- , neg )

    // friend auto &operator+=( auto &a, auto &&b ) { a = a + FORWARD( b ); return a; }
    // friend auto &operator/=( auto &a, auto &&b ) { a = a / FORWARD( b ); return a; }

    #undef ADD_BIN_OP
    #undef ADD_UNA_OP
};

END_VFS_NAMESPACE
