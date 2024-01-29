#pragma once

#include "../support/IsSpecializationOf.h"
#include "../support/CompilationFlags.h"
#include "ArrayBlockImpl.h"
#include "VecImpl.h"
//#include "ArrayLike.h"

BEG_VFS_NAMESPACE

/// Set of array blocks that form an array of potentially different types, alignments, ...
template<
    class BlockTypes = CtTypeList<VecImpl<ArrayBlockImpl<double>,1>>,
    class CtSizes = CtIntList<-1>,
    class Memory = Memory_Cpu,
    class DefaultValue = void
>
class ArrayImpl;

#define DTP template<class BlockTypes,class CtSizes,class Memory,class DefaultValue>
#define UTP ArrayImpl<BlockTypes,CtSizes,Memory,DefaultValue>

DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<BlockTypes>() ); f( CtType<CtSizes>() ); f( CtType<Memory>() ); f( CtType<DefaultValue>() ); }
DTP void get_compilation_flags( CompilationFlags &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/support/containers/ArrayImpl.h" ); }
DTP auto template_type_name( CtType<UTP> ) { return "ArrayImpl"; }
DTP auto block_types_of( CtType<UTP> ) { return BlockTypes{}; }
DTP auto ct_sizes_of( CtType<UTP> ) { return CtSizes{}; }
DTP auto memory_of( const UTP &a ) { return a.memory; }

#undef DTP
#undef UTP

TT concept IsA_ArrayImpl = IsSpecializationOf<ArrayImpl,T>::value;

// with exactly 1 block -------------------------------------------------------------------------------
template<class BlockType,class CtSizes>
class ArrayImpl<CtTypeList<VecImpl<BlockType,1>>,CtSizes,Memory_Cpu,void> : public WithDefaultOperators {
public:
};



// /// homogeneous dynamic vector ---------------------------------------------------------------------
// template<class Item,int alig>
// class ArrayImpl<CtTypeList<Item>,CtIntList<-1>,CtIntList<alig>,Memory_Cpu> {
// public:
//     // /**/             ArrayImpl         ( FromSizeAndUninitializedItemValues, Memory_Cpu, PI size );
//     // /**/             ArrayImpl         ( FromSizeAndExternalItemValues     , Memory_Cpu, PI size, const void *data_ptr );
//     // TTA              ArrayImpl         ( FromSizeAndInitItemFunction       , Memory_Cpu, PI size, const T &func, const A &...add_args );
//     // TA               ArrayImpl         ( FromSizeAndItemValue              , Memory_Cpu, PI size, const A &... value );
//     // /**/             ArrayImpl         ( FromReservationSize               , Memory_Cpu, PI capa );
//     // /**/             ArrayImpl         ( FromIterator                      , Memory_Cpu, auto iter_data, IteratorLike auto iter_size );
//     // /**/             ArrayImpl         ( FromIterator                      , Memory_Cpu, auto iter_data, auto iter_size );
//     /**/               ~ArrayImpl         ();

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
// class alignas( alig * alignof( Item ) ) ArrayImpl<CtTypeList<Item>,CtIntList<ct_size>,CtIntList<alig>,Memory_Cpu> {
// public:
//     static constexpr PI data_size            = ct_size * sizeof( Item );

//     // TTA                 ArrayImpl         ( FromSizeAndInitItemFunction, Memory_Cpu, PI size, const T &func, const A &...add_args );
//     // TA                  ArrayImpl         ( FromSizeAndItemValues      , Memory_Cpu, A&&... values );
//     // TA                  ArrayImpl         ( FromSizeAndItemValue       , Memory_Cpu, auto, A&&... ctor_args );
//     // /**/                ArrayImpl         ( FromIterator               , Memory_Cpu, auto iter_data, IteratorLike auto iter_size );
//     // /**/                ArrayImpl         ( FromIterator               , Memory_Cpu, auto iter_data, auto iter_size );
//     /**/                ArrayImpl         ();
//     /**/               ~ArrayImpl         ();

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
// class ArrayImpl<SL<Items...>,NL<ct_size>,NL<alig>,Memory_Cpu> {
// public:
//     using               TupleType            = Tuple<Items...>;
//     static constexpr PI data_size            = sizeof( TupleType );

//     TA                  ArrayImpl         ( FromItemValues, A&&... values );
//     /**/               ~ArrayImpl         ();

//     TT void             for_each_item        ( T &&func ) const;
//     TT void             for_each_item        ( T &&func );
//     DisplayerItem*      display              ( Displayer &ds ) const;
//     PI                  size                 () const { return sizeof...( Items ); }

//     char                data[ data_size ];
// };

//// --------------------------------------------------------------------------------------------------
//// ----------------------------------------- operations ---------------------------------------------
//// --------------------------------------------------------------------------------------------------
// auto operator+( const IsA_ArrayImpl auto &a, const IsA_ArrayImpl auto &b ) {
//     auto res = a;
//     for( PI i = 0; i < a.size(); ++i )
//         res[ i ] += b[ i ];
//     return res;
// }

// auto operator-( const IsA_ArrayImpl auto &a, const IsA_ArrayImpl auto &b ) {
//     auto res = a;
//     for( PI i = 0; i < a.size(); ++i )
//         res[ i ] -= b[ i ];
//     return res;
// }

// auto operator/( const IsA_ArrayImpl auto &a, const IsA_ArrayImpl auto &b ) { auto res = a; for( PI i = 0; i < a.size(); ++i ) res[ i ] /= b[ i ]; return res; }
// auto operator/( const IsA_ArrayImpl auto &a, const NotArrayLike auto &b ) { auto res = a; for( PI i = 0; i < a.size(); ++i ) res[ i ] /= b; return res; }

}

#include "ArrayImpl.tcc" // IWYU pragma: export
