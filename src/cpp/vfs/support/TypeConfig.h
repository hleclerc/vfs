#pragma once

#include "common_macros.h"
#include <string_view>
#include <cstdint>
#include <memory>
#include <string>

BEG_VFS_NAMESPACE

// number types
using       PI64                       = std::conditional< sizeof( unsigned long ) == 8, unsigned long, unsigned long long >::type;
using       PI32                       = std::uint32_t;
using       PI32                       = std::uint32_t;
using       PI16                       = std::uint16_t;
using       PI8                        = std::uint8_t;

using       SI64                       = std::conditional< sizeof( signed long ) == 8, signed long, signed long long >::type;
using       SI32                       = std::int32_t;
using       SI16                       = std::int16_t;
using       SI8                        = std::int8_t;

using       Bool                       = bool;

using       Byte                       = std::byte;

using       PI                         = std::conditional< sizeof( void * ) == 8, PI64, PI32 >::type;
using       SI                         = std::conditional< sizeof( void * ) == 8, SI64, SI32 >::type;

using       FP64                       = double;
using       FP32                       = float;

// common std types
TT using    UniquePtr                  = std::unique_ptr<T>;
using       StrView                    = std::string_view;
TA using    Tuple                      = std::tuple<A...>;
TUV using   Pair                       = std::pair<U,V>;
using       Str                        = std::string;

// // args for ctor selection
// #define DECL_VOID_STRUCT_TTT( NAME, ... ) template<class T> struct NAME { static auto base_type_name() { return #NAME; } static void for_each_template_arg( auto &&f ) { f( S<T>() ); } }
#define DECL_VOID_STRUCT_STD( NAME ) struct NAME { static auto type_name() { return #NAME; } }

// DECL_VOID_STRUCT_STD( FromSizeAndUninitializedItemValues ); ///<
// DECL_VOID_STRUCT_STD( FromSizeAndExternalItemValues  ); ///< will make a ref
// DECL_VOID_STRUCT_STD( FromSizeAndItemIndexFunction   ); ///< fill with a init function
// DECL_VOID_STRUCT_STD( FromSizeAndInitItemFunction    ); ///< fill with a init function
// DECL_VOID_STRUCT_STD( FromSizeAndItemValues          ); ///< will make a copy
// DECL_VOID_STRUCT_STD( FromSizeAndItemValue           ); ///< fill-like
// DECL_VOID_STRUCT_TTT( FromTypedItemValues            ); ///< will make a copy, with a forced type
// DECL_VOID_STRUCT_STD( FromMovableIterator            );
// DECL_VOID_STRUCT_STD( FromIterator                   );
// DECL_VOID_STRUCT_STD( FromExternalValue              ); ///< will make a ref
// DECL_VOID_STRUCT_STD( FromInitFunction               );
// DECL_VOID_STRUCT_STD( Uninitialized                  );

DECL_VOID_STRUCT_STD( FromSizeAndInitFunctionOnIndex );
DECL_VOID_STRUCT_STD( FromTypeAndCtorArguments       );
DECL_VOID_STRUCT_STD( FromOperationOnItemsOf         );
DECL_VOID_STRUCT_STD( FromSizeAndItemValue           );
DECL_VOID_STRUCT_STD( FromReservationSize            ); ///<
DECL_VOID_STRUCT_STD( FromItemValues                 ); ///< will make copies
DECL_VOID_STRUCT_STD( FromItemValue                  );
DECL_VOID_STRUCT_STD( FromIterator                   ); ///<
DECL_VOID_STRUCT_STD( FromPointer                    ); ///<
DECL_VOID_STRUCT_STD( FromValue                      );
DECL_VOID_STRUCT_STD( FromSize                       );

// // common concepts
// TT concept IsNotA_Unitialized = std::is_same_v<T,Uninitialized> == false;
TT concept HasSizeAndAccess   = requires( const T &t ) { t.size(); } && ( requires( const T &t ) { t.begin(); } || requires( const T &t ) { t[ 0 ]; } );
TT concept HasSizeAndSelect   = requires( const T &t ) { t.size(); t[ 0 ]; };
TT concept HasBeginAndEnd     = requires( const T &t ) { t.begin(); t.end(); };
TT concept ScalarLike         = requires( const T &t ) { t * t; };
TT concept ListLike           = HasSizeAndSelect<T> || HasBeginAndEnd<T>;
TT concept StrLike            = std::is_convertible_v<T,std::string_view>;
// TT concept IteratorLike       = requires( T &t ) { *( t++ ); };
// TT concept FunctionLike       = std::is_function_v<std::decay_t<T>>;
// TT concept IsA_S              = IsSpecializationOf<S,T>::value;

#undef DECL_VOID_STRUCT_STD

END_VFS_NAMESPACE

// common includes
#include "ASSERT.h" // IWYU pragma: export
#include "ERROR.h" // IWYU pragma: export
#include "TODO.h" // IWYU pragma: export
#include "P.h" // IWYU pragma: export

#include "conv.h" // IWYU pragma: export

#include "CtStringList.h" // IWYU pragma: export
#include "CtTypeList.h" // IWYU pragma: export
#include "CtIntList.h" // IWYU pragma: export
#include "Void.h" // IWYU pragma: export
// #include "NL.h" // IWYU pragma: export
// #include "SL.h" // IWYU pragma: export

// #include "initialize.h" // IWYU pragma: export
// #include "convert.h" // IWYU pragma: export

#include "Compare.h" // IWYU pragma: export
