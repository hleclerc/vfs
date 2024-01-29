#pragma once

#include "../../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

/// Set of array blocks that form an array of potentially different types, alignments, ...
template<class Memory,class... Blocks>
class LocalArrayImpl;

void for_each_template_arg( auto &&f ) { f( CtType<Memory>() ); f( CtType<Block>() ); ( f( CtType<NextBlocks>() ), ... ); }
auto template_type_name   () { return "GenericArray"; }
void get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/support/containers/GenericArray.h" ); }


// with tail
template<class Memory,class Block,class... NextBlocks>
class GenericArray<Memory,Block,NextBlocks...> {
public:
    using       Next                 = GenericArray<Memory,NextBlocks...>;


    Block       block;               ///<
    Next        next;                ///<
};

// last one
template<class Memory>
class GenericArray<Memory> {
public:
    static void for_each_template_arg( auto &&f ) { f( CtType<Memory>() ); }
    static auto template_type_name   () { return "GenericArray"; }
    static auto get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/support/containers/GenericArray.h" ); }
};

TT concept IsA_GenericArray = IsSpecializationOf<GenericArray,T>::value;



// #define DTPV template<class Memory,class... Blocks,class V>
// #define DTP template<class Memory,class... Blocks>
// #define UTP GenericArray<Memory,Blocks...>


// #undef DTPV
// #undef DTP
// #undef UTP

// /// homogeneous dynamic vector ---------------------------------------------------------------------
// template<class Item,int alig>
// class GenericArray<CtTypeList<Item>,CtIntList<-1>,CtIntList<alig>,Memory_Cpu> {
// public:
//     // /**/                GenericArray         ( FromSizeAndUninitializedItemValues, Memory_Cpu, PI size );
//     // /**/                GenericArray         ( FromSizeAndExternalItemValues     , Memory_Cpu, PI size, const void *data_ptr );
//     // TTA                 GenericArray         ( FromSizeAndInitItemFunction       , Memory_Cpu, PI size, const T &func, const A &...add_args );
//     // TA                  GenericArray         ( FromSizeAndItemValue              , Memory_Cpu, PI size, const A &... value );
//     // /**/                GenericArray         ( FromReservationSize               , Memory_Cpu, PI capa );
//     // /**/                GenericArray         ( FromIterator                      , Memory_Cpu, auto iter_data, IteratorLike auto iter_size );
//     // /**/                GenericArray         ( FromIterator                      , Memory_Cpu, auto iter_data, auto iter_size );
//     /**/               ~GenericArray         ();

//     Item&               value_at_off_and_inds( CtType<Item>, PI off, PI ind );
//     TT void             for_each_off_and_inds( T &&func ) const;
//     const Item&         operator[]           ( const PI index ) const { return select( index ); }
//     Item&               operator[]           ( const PI index ) { return select( index ); }
//     DisplayItem*        display              ( Displayer &ds ) const;
//     const Item&         select               ( PI index ) const { return data_[ index ]; }
//     Item&               select               ( PI index ) { return data_[ index ]; }
//     Item*               begin                () const { return data_; }
//     PI                  size                 () const { return size_; }
//     Item*               data                 () const { return data_; }
//     Item*               end                  () const { return data_ + size_; }

//     TA void             push_back_in_reserved( A&&...args ) { new ( data_ + size_++ ) Item( std::forward<A>( args )... ); }
//     void                raw_size_assignation ( PI size );

//     static Item*        allocate             ( PI nb_items );

//     void                init_with_room_for   ( PI capa );
//     void                clear                ();
//     void                free                 ();

//     PI                  size_;               ///<
//     PI                  capa_;               ///<
//     Item*               data_;               ///<
// };

// /// static vector with 1 type ----------------------------------------------------------
// template<class Item,int ct_size,int alig>
// class alignas( alig * alignof( Item ) ) GenericArray<CtTypeList<Item>,CtIntList<ct_size>,CtIntList<alig>,Memory_Cpu> {
// public:
//     static constexpr PI data_size            = ct_size * sizeof( Item );

//     // TTA                 GenericArray         ( FromSizeAndInitItemFunction, Memory_Cpu, PI size, const T &func, const A &...add_args );
//     // TA                  GenericArray         ( FromSizeAndItemValues      , Memory_Cpu, A&&... values );
//     // TA                  GenericArray         ( FromSizeAndItemValue       , Memory_Cpu, auto, A&&... ctor_args );
//     // /**/                GenericArray         ( FromIterator               , Memory_Cpu, auto iter_data, IteratorLike auto iter_size );
//     // /**/                GenericArray         ( FromIterator               , Memory_Cpu, auto iter_data, auto iter_size );
//     /**/                GenericArray         ();
//     /**/               ~GenericArray         ();

//     TT void             for_each_item        ( T &&func ) const;
//     TT void             for_each_item        ( T &&func );
//     const Item&         operator[]           ( const PI index ) const { return select( index ); }
//     Item&               operator[]           ( const PI index ) { return select( index ); }
//     DisplayItem*        display              ( Displayer &ds ) const;
//     const Item&         select               ( PI index ) const;
//     Item&               select               ( PI index );
//     auto                size                 () const { return N<ct_size>(); }
//     const Item*         data                 () const { return reinterpret_cast<const Item *>( data_ ); }
//     Item*               data                 () { return reinterpret_cast<Item *>( data_ ); }

//     char                data_[ data_size ];
// };

// /// static vector ---------------------------------------------------------------------
// template<class... Items,int ct_size,int alig>
// class GenericArray<SL<Items...>,NL<ct_size>,NL<alig>,Memory_Cpu> {
// public:
//     using               TupleType            = Tuple<Items...>;
//     static constexpr PI data_size            = sizeof( TupleType );

//     TA                  GenericArray         ( FromItemValues, A&&... values );
//     /**/               ~GenericArray         ();

//     TT void             for_each_item        ( T &&func ) const;
//     TT void             for_each_item        ( T &&func );
//     DisplayerItem*      display              ( Displayer &ds ) const;
//     PI                  size                 () const { return sizeof...( Items ); }

//     char                data[ data_size ];
// };

//// --------------------------------------------------------------------------------------------------
//// ----------------------------------------- operations ---------------------------------------------
//// --------------------------------------------------------------------------------------------------
// auto operator+( const IsA_GenericArray auto &a, const IsA_GenericArray auto &b ) {
//     auto res = a;
//     for( PI i = 0; i < a.size(); ++i )
//         res[ i ] += b[ i ];
//     return res;
// }

// auto operator-( const IsA_GenericArray auto &a, const IsA_GenericArray auto &b ) {
//     auto res = a;
//     for( PI i = 0; i < a.size(); ++i )
//         res[ i ] -= b[ i ];
//     return res;
// }

// auto operator/( const IsA_GenericArray auto &a, const IsA_GenericArray auto &b ) { auto res = a; for( PI i = 0; i < a.size(); ++i ) res[ i ] /= b[ i ]; return res; }
// auto operator/( const IsA_GenericArray auto &a, const NotArrayLike auto &b ) { auto res = a; for( PI i = 0; i < a.size(); ++i ) res[ i ] /= b; return res; }

// auto sp( const IsA_GenericArray auto &a, const IsA_GenericArray auto &b ) {
//     auto res = a[ 0 ] * b[ 0 ];
//     for( PI i = 1; i < a.size(); ++i )
//         res += a[ i ] * b[ i ];
//     return res;
// }

// auto max( const IsA_GenericArray auto &a ) {
//     using std::max;
//     auto res = a[ 0 ];
//     for( PI i = 1; i < a.size(); ++i )
//         res = max( res, a[ i ] );
//     return res;
// }

}

#include "ArrayImpl.tcc" // IWYU pragma: export
