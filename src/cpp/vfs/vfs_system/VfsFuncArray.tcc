#pragma once

#include "VfsFuncArray.h"

BEG_VFS_NAMESPACE

// generic case --------------------------------------------------------------
#define DTP template<class Callable,class TupleOfKeys>
#define UTP VfsFuncArray<Callable,TupleOfKeys>

DTP UTP::VfsFuncArray( Callable *init ) : init( init ) {
}

DTP Callable** UTP::operator()( auto &&tuple_of_keys ) {
    TupleOfKeys key( FromTupleValues(), FORWARD( tuple_of_keys ) );
    auto iter = map.find( key );
    if ( iter == map.end() )
        iter = map.insert( iter, { std::move( key ), init } );
    return &iter->second;
}

#undef DTP
#undef UTP

/// 0 arg -------------------------------------------------------
#define DTP template<class Callable>
#define UTP VfsFuncArray<Callable,Tup<>>

DTP UTP::VfsFuncArray( Callable *init ) {
    ptr = init;
}

DTP Callable** UTP::operator()( const auto &tuple_of_keys ) {
    return &ptr;
}

#undef DTP
#undef UTP

/// 1 indexable arg -------------------------------------------------------
#define DTP template<class Callable,IsAVfsKeyWithIndexAndArraySize Key>
#define UTP VfsFuncArray<Callable,Tup<Key>>

DTP UTP::VfsFuncArray( Callable *init ) {
    size = Key::array_size();
    ptr = new Callable *[ size ];
    for( std::size_t i = 0; i < size; ++i )
        ptr[ i ] = init;
}

DTP UTP::~VfsFuncArray() {
    delete [] ptr;
}

DTP Callable** UTP::operator()( const auto &tuple_of_keys ) {
    return ptr + tuple_of_keys.head.value();
}

#undef DTP
#undef UTP

/// 2 indexable args ------------------------------------------------------
#define DTP template<class Callable,IsAVfsKeyWithIndexAndArraySize Key_0,IsAVfsKeyWithIndexAndArraySize Key_1>
#define UTP VfsFuncArray<Callable,Tup<Key_0,Key_1>>

DTP UTP::VfsFuncArray( Callable *init ) {
    size_0 = Key_0::array_size();
    size_1 = Key_1::array_size();
    ptr = new Callable *[ size_0 * size_1 ];
    for( std::size_t i = 0; i < size_0 * size_1; ++i )
        ptr[ i ] = init;
}

DTP UTP::~VfsFuncArray() {
    delete [] ptr;
}

DTP Callable** UTP::operator()( const auto &tuple_of_keys ) {
    return ptr + tuple_of_keys.head.value() + size_0 * tuple_of_keys.tail.head.value();
}

#undef DTP
#undef UTP

// /// 2 arg --------------------------------------------------------------
// #define DTP template<class Callable>
// #define UTP VfsFuncArray<Callable,2,false>

// DTP TA UTP::VfsFuncArray( Callable *init, S<A>... args ) {
//     GetVfsObjects::apply_types( [&]( auto a, auto b ) {
//         auto &table_a = StaticStorage<VfsTypeTable,typename decltype( a )::T::VfsObjectId>::value;
//         auto &table_b = StaticStorage<VfsTypeTable,typename decltype( b )::T::VfsObjectId>::value;
//         size_a = table_a.current_vfs_func_array_size;
//         size_b = table_b.current_vfs_func_array_size;
//     }, args... );

//     PI tot_size = size_a * size_b;
//     ptr = new Callable *[ tot_size ];
//     for( std::size_t i = 0; i < tot_size; ++i )
//         ptr[ i ] = init;
// }

// DTP UTP::~VfsFuncArray() {
//     delete [] ptr;
// }

// DTP TA Callable** UTP::operator()( const A &... args ) {
//     return GetVfsObjects::apply_values( [&]( const auto &a, const auto &b ) {
//         return ptr + a.vfs_data.get_vfs_func_type_index() * size_b + b.vfs_data.get_vfs_func_type_index();
//     }, args... );
// }

// #undef DTP
// #undef UTP


// /// n args --------------------------------------------------------------
// #define DTP template<class Callable,int nb_vfs_args>
// #define UTP VfsFuncArray<Callable,nb_vfs_args,false>

// DTP TA UTP::VfsFuncArray( Callable *init, S<A>... ) : init( init ) {
// }

// DTP TA Callable** UTP::operator()( const A &... args ) {
//     return GetVfsObjects::apply_values( [&]( const auto &... vo ) {
//         Key key{ vo.vfs_data.get_vfs_func_type_index()... };
//         auto iter = map.find( key );
//         if ( iter == map.end() )
//             iter = map.insert( iter, { std::move( key ), init } );
//         return &iter->second;
//     }, args... );
// }

// #undef DTP
// #undef UTP

// /// with ct args --------------------------------------------------------------
// #define DTP template<class Callable,int nb_vfs_args>
// #define UTP VfsFuncArray<Callable,nb_vfs_args,true>

// DTP TA UTP::VfsFuncArray( Callable *init, S<A>... ) : init( init ) {
// }

// DTP TA Callable** UTP::operator()( const A &... args ) {
//     auto append_params = []( Str &sr, const auto &arg ) {
//         if constexpr( IsA_CtValue<decltype(arg)> )
//             arg.get_summary( sr );
//     };
//     Str sr;
//     ( append_params( sr, args ), ... );

//     auto iter = func_map.find( sr );
//     if ( iter == func_map.end() )
//         iter = func_map.insert( iter, { sr, std::unique_ptr<TargetFunc>{ new TargetFunc{ init, S<A>()... } } } );

//     return (*iter->second)( args... );
// }

// #undef DTP
// #undef UTP

END_VFS_NAMESPACE
