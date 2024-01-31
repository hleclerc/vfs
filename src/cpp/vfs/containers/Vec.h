#pragma once

#include "../support/size_and_lexical_comparison.h"
#include "../support/TypeConfig.h"
#include "Memory/Memory_Cpu.h"

BEG_VFS_NAMESPACE

/// basic sequence of Items with potentially static size, potential room for local data
/// Alignement is specified in bytes and can be = 0.
///
///
/// This specialization is for static vectors
template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
class alignas( std::max( PI( alignment ), alignof( Item ) ) ) Vec : public WithDefaultOperators {
public:
    static auto         with_item_type( auto item_type ) { return CtType< Vec<typename VALUE_IN_DECAYED_TYPE_OF(item_type),static_size> >{}; }

    /**/                Vec           ( FromOperationOnItemsOf, auto op_name, auto nb_indices_to_take, auto &&...operands );
    /**/                Vec           ( FromItemValues, auto &&...values );
    /**/                Vec           ( FromItemValue, auto &&...ctor_args );
    /**/                Vec           ( FromIterator, auto iter );
    TT                  Vec           ( const std::initializer_list<T> &lst );
    /**/                Vec           ( const HasSizeAndAccess auto &l );
    /**/                Vec           ( const Vec &that );
    /**/                Vec           ( Vec && );
    /**/                Vec           ();
    /**/               ~Vec           ();

    Vec&                operator=     ( const Vec & );
    Vec&                operator=     ( Vec && );

    const Item&         operator[]    ( PI index ) const;
    Item&               operator[]    ( PI index );
    const Item&         operator()    ( PI index ) const;
    Item&               operator()    ( PI index );
    PI                  size_tot      () const { return size(); }
    const Item*         begin         () const { return data(); }
    Item*               begin         () { return data(); }
    const Item*         data          ( PI index ) const;
    Item*               data          ( PI index );
    const Item*         data          () const;
    Item*               data          ();
    const Item&         back          () const { return operator[]( size() - 1 ); }
    Item&               back          () { return operator[]( size() - 1 ); }
    const Item*         end           () const { return begin() + size(); }
    Item*               end           () { return begin() + size(); }

    CtInt<static_size>  size          ( PI d ) const { return {}; }
    CtInt<static_size>  size          () const { return {}; }

    static constexpr PI nbch          = static_size * sizeof( Item );
    char                data_         [ nbch ]; ///<
};

// dynamic size, items fully on the heap
template<class Item,int alignment>
class Vec<Item,-1,0,alignment,true> : public WithDefaultOperators {
public:
    /**/                Vec         ( FromSizeAndInitFunctionOnIndex, PI size, auto &&func );
    /**/                Vec         ( FromOperationOnItemsOf, auto &&a, auto &&operation );
    /**/                Vec         ( FromSizeAndItemValue, PI size, auto &&...ctor_args );
    /**/                Vec         ( FromReservationSize, PI capa, PI raw_size = 0 );
    /**/                Vec         ( FromItemValues, auto &&...values );
    /**/                Vec         ( FromSize, PI size );
    /**/                Vec         ( const std::initializer_list<Item> &l );
    /**/                Vec         ( const HasSizeAndAccess auto &l );
    /**/                Vec         ( const Vec & );
    /**/                Vec         ( Vec && );
    /**/                Vec         ();
    /**/               ~Vec         ();

    static Vec          range       ( Item end );

    Vec&                operator=   ( const Vec &that );
    Vec&                operator=   ( Vec &&that );

    Vec&                operator<<  ( auto &&value ) { push_back( FORWARD( value) ); return *this; }

    const Item&         operator[]  ( PI index ) const;
    Item&               operator[]  ( PI index );
    PI                  size_tot    () const { return size(); }
    const Item*         begin       () const { return data(); }
    Item*               begin       () { return data(); }
    const Item*         data        ( PI index ) const;
    Item*               data        ( PI index );
    const Item*         data        () const;
    Item*               data        ();
    const Item&         back        () const { return operator[]( size() - 1 ); }
    Item&               back        () { return operator[]( size() - 1 ); }
    const Item*         end         () const { return begin() + size(); }
    Item*               end         () { return begin() + size(); }

    bool                empty       () const { return size_ == 0; }
    PI                  size        ( PI d ) const { return size(); }
    PI                  size        () const;

    Item                pop_back_val();
    void                push_back_br( auto&&...args ); ///< push_back with Item{ FORWARD( args )... }
    void                push_back   ( auto&&...args ); ///< push_back with Item( FORWARD( args )... )
    void                reserve     ( PI capa );
    void                resize      ( PI size, auto&&...ctor_args );
    void                append      ( auto &&that );
    void                clear       ();

    void                copy_data_to( void *data ) const;

    static Item*        allocate    ( PI nb_items );

    Item*               data_;    ///<
    PI                  size_;    ///<
    PI                  capa_;    ///<
};

// --------------------------------------------------------------------------------------------------------------------------------------------
#define DTP template<class Item,int static_size,int local_size,int alignment,bool allow_heap>
#define UTP Vec<Item,static_size,local_size,alignment,allow_heap>

DTP auto get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/containers/Vec.h" ); }
DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<Item>() ); f( CtInt<static_size>() ); f( CtInt<local_size>() ); f( CtInt<alignment>() ); f( CtInt<allow_heap>() ); }
DTP auto template_type_name( CtType<UTP> ) { return "Vec"; }
DTP auto block_types_of( CtType<UTP> ) { return CtTypeList<Vec<UTP,1>>(); }
DTP auto ct_sizes_of( CtType<UTP> ) { return CtIntList<static_size>(); }
DTP auto memory_of( const UTP &a ) { return Memory_Cpu(); }

DTP constexpr auto tensor_order( CtType<UTP> ) { return CtInt<1>(); }

Ti auto VecType_for( auto item_type, CtInt<i> ) { return CtType<Vec<typename VALUE_IN_DECAYED_TYPE_OF( item_type ),i>>(); }
auto VecType_for( auto item_type, PI ) { return CtType<Vec<typename VALUE_IN_DECAYED_TYPE_OF( item_type )>>(); }

END_VFS_NAMESPACE

namespace std {
DTP struct less<VFS_NAMESPACE::UTP> { // : public binary_function<VFS_NAMESPACE::UTP, VFS_NAMESPACE::UTP, bool>
    constexpr bool operator()( const VFS_NAMESPACE::UTP &a, const VFS_NAMESPACE::UTP &b ) const {
        return size_and_lexical_comparison( a, b );
    }
};
} // namespace std

#undef DTP
#undef UTP

#include "Vec.tcc" // IWYU pragma: export

#include "make_ArrayImpl_from_unary_operations.h" // IWYU pragma: export
#include "make_ArrayImpl_from_binary_operations.h" // IWYU pragma: export
