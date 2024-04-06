#pragma once

#include "TypeConfig.h"

BEG_VFS_NAMESPACE

// /// like operator= with a check that throw an exception to change the underlying type
// TT CtInt<1> reassign( T &dst, const T &src ) { dst = src; return {}; }
// TT CtInt<1> reassign( T &dst, T &&src ) { dst = std::move( src ); return {}; }

// //TT CtInt<1> reassign( FP32 &dst, SI32 src ) { dst = std::move( src ); return {}; }

// /// generic case => make a conversion
// bool reassign( auto &dst, auto &&src ) {
//     if constexpr ( scalar_class( CT_DECAYED_TYPE_OF( dst ) ) >= scalar_class( CT_DECAYED_TYPE_OF( src ) ) ) {
//         dst = src;
//         return true;
//     }
//     return dst == src;
// }
void reassign( auto &self, auto &&that ) {
    self = FORWARD( that );
}

END_VFS_NAMESPACE
