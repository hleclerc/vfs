#pragma once

#include "WithDefaultOperators.h"
#include "TypeConfig.h"
#include "CtInt.h"
// #include "../support/reassign.h"
// #include "Memory/Memory_Cpu.h"

BEG_VFS_NAMESPACE

/// basic sequence of Items with potentially static size and potential room for local data
/// Alignement is specified in bytes and can be = 0.
///
///
/// This specialization is for static vectors
template<class Item,int static_size=-1>
class Vec : public WithDefaultOperators {
public:
    // static auto         with_item_type( auto item_type ) { return CtType< Vec<typename VALUE_IN_DECAYED_TYPE_OF(item_type),static_size> >{}; }

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
template<class Item>
class Vec<Item,-1> : public WithDefaultOperators {
public:
    /**/                Vec         ( FromSizeAndInitFunctionOnIndex, PI size, auto &&func );
    /**/                Vec         ( FromOperationOnItemsOf, auto &&functor, auto nb_inds_to_take, auto &&...lists );
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

    bool                contains    ( const auto &v ) const { for( const auto &r : *this ) if ( r == v ) return true; return false; }
    bool                empty       () const { return size_ == 0; }
    PI                  size        ( PI d ) const { return size(); }
    PI                  size        () const;

    Item                pop_back_val();
    Item*               push_back_br( auto&&...args ); ///< push_back with Item{ FORWARD( args )... }
    Item*               push_back   ( auto&&...args ); ///< push_back with Item( FORWARD( args )... )
    void                reserve     ( PI capa );
    void                resize      ( PI size, auto&&...ctor_args );
    void                append      ( auto &&that );
    void                clear       ();

    void                copy_data_to( void *data ) const;
    TUV void            set_item    ( CtType<U> array_type, CtType<V> item_type, auto &&value, const auto &index ) {
        if ( ! reassign( operator[]( index ), FORWARD( value ) ) ) {
            using Dst = VALUE_IN_DECAYED_TYPE_OF( type_promote( CtString<"reassign">(),
                vfs_dt_impl_type( item_type, FORWARD( value ) ),
                CtType<Item>()
            ) );
            P( type_name( CT_DECAYED_TYPE_OF( value ) ), type_name<Dst>() );
            throw typename U::template TypeException<Vec<Dst>,Vec>{ std::move( *this ) };
        }
    }
    const Item&         get_item    ( const auto &index ) const { return operator[]( index ); }

    static Item*        allocate    ( PI nb_items );

    Item*               data_;    ///<
    PI                  size_;    ///<
    PI                  capa_;    ///<
};

// --------------------------------------------------------------------------------------------------------------------------------------------
#define DTP template<class Item,int static_size>
#define UTP Vec<Item,static_size>

DTP auto           get_compilation_flags( auto &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/containers/Vec.h" ); }
DTP void           for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<Item>() ); f( CtInt<static_size>() ); }
DTP auto           template_type_name   ( CtType<UTP> ) { return "Vec"; }
DTP constexpr auto tensor_order         ( CtType<UTP> ) { return CtInt<1>(); }

DTP struct         StaticSizesOf<UTP> { using value = PrimitiveCtIntList<static_size>; };
DTP struct         ItemTypeOf<UTP> { using value = Item; };

// DTP constexpr auto ct_sizes_of( CtType<UTP> ) { return CtIntList<static_size>(); }
// DTP auto memory_of( const UTP &a ) { return Memory_Cpu(); }

// DTP constexpr auto block_types_of( CtType<UTP> ) { return CtTypeList<Vec<UTP,1>>(); }

Ti auto VecType_for( auto item_type, CtInt<i> ) { return CtType<Vec<typename VALUE_IN_DECAYED_TYPE_OF( item_type ),i>>(); }
auto VecType_for( auto item_type, PI ) { return CtType<Vec<typename VALUE_IN_DECAYED_TYPE_OF( item_type )>>(); }

END_VFS_NAMESPACE

#undef DTP
#undef UTP

template<class ItemType,int static_size>
struct ArrayTypeFor<ItemType,PrimitiveCtIntList<static_size>,1> {
    using value = Vec<ItemType,static_size>;
};

#include "Vec.tcc" // IWYU pragma: export
