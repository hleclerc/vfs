#pragma once

#include "VfsDtWrap_.h"
#include "VfsDtWrap.h"
#include "VfsDtType.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

#define DTP template<class Object_,int size_,int alig_>
#define UTP VfsDtWrap<Object_,size_,alig_>

DTP TT void UTP::construct( FromTypeAndCtorArguments, CtType<T> t, auto &&...ctor_args ) {
    new ( this ) VfsDtWrap_<Object,T>( FORWARD( ctor_args )... );
}

DTP TT void UTP::construct( FromPointer, T &&pointer ) {
    TODO;
//     auto &type = StaticStorage<VfsDtType<Object,T,1>>::value;
//     instantiated_type_index = type.instantiated_type_index;
//     global_type_index = type.global_type_index;

//     auto t = CT_DECAYED_TYPE_OF( pointer );
//     if ( not_enough_room_for( t ) )
//         void_ptr() = std::malloc( sizeof( T ) );
//     new ( &cast( t ) ) T( FORWARD( pointer ) );
}

DTP TT void UTP::construct( FromValue, T &&value ) {
     construct( FromTypeAndCtorArguments(), CT_DECAYED_TYPE_OF( value ), FORWARD( value ) );
}

// DTP void *const &UTP::void_ptr() const {
//     constexpr int off = ( 2 * sizeof( PI32 ) + alignof( void * ) - 1 ) / alignof( void * ) * alignof( void * );
//     return *reinterpret_cast<void * const *>( reinterpret_cast<const char *>( this ) + off );
// }

// DTP void *&UTP::void_ptr() {
//     constexpr int off = ( 2 * sizeof( PI32 ) + alignof( void * ) - 1 ) / alignof( void * ) * alignof( void * );
//     return *reinterpret_cast<void **>( reinterpret_cast<char *>( this ) + off );
// }

// DTP TT const T &UTP::cast( CtType<T> ) const {
//     constexpr int off = ( 2 * sizeof( PI32 ) + alignof( T ) - 1 ) / alignof( T ) * alignof( T );
//     constexpr bool external_content = alignof( T ) > data_alig || off + sizeof( T ) > sizeof( *this );
//     return *reinterpret_cast<const T *>( external_content ? void_ptr() : reinterpret_cast<const char *>( this ) + off );
// }

// DTP TT T &UTP::cast( CtType<T> ) {
//     constexpr int off = ( 2 * sizeof( PI32 ) + alignof( T ) - 1 ) / alignof( T ) * alignof( T );
//     constexpr bool external_content = alignof( T ) > data_alig || off + sizeof( T ) > sizeof( *this );
//     return *reinterpret_cast<T *>( external_content ? void_ptr() : reinterpret_cast<char *>( this ) + off );
// }

// DTP TT const T &UTP::cast() const {
//     return cast( CtType<T>() );
// }

// DTP TT T &UTP::cast() {
//     return cast( CtType<T>() );
// }


#undef DTP
#undef UTP

END_VFS_NAMESPACE
