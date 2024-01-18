#pragma once

#include "../support/default_operators.h"
#include "../support/CompilationFlags.h"
#include "Memory/Memory_Cpu.h"

namespace Vfs {

/// basic vector of Items with potentially static size, potential room for local data
/// Alignement is specified in bytes and can be = 0.
///
///
/// This specialization is for static vectors
template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
class alignas( std::max( PI( alignment ), alignof( Item ) ) ) VecImpl : public WithDefaultOperators {
public:
    static auto         with_item_type( auto item_type ) { return CtType< VecImpl<typename GET_DT_VALUE(item_type),static_size> >{}; }

    /**/                VecImpl       ( FromOperationOnItemsOf, auto op_name, auto nb_indices_to_take, auto &&...operands );
    /**/                VecImpl       ( FromItemValues, auto &&...values );
    /**/                VecImpl       ( FromItemValue, auto &&...ctor_args );
    /**/                VecImpl       ( FromIterator, auto iter );
    TT                  VecImpl       ( const std::initializer_list<T> &lst );
    /**/                VecImpl       ( const HasSizeAndAccess auto &l );
    /**/                VecImpl       ( const VecImpl &that );
    /**/                VecImpl       ( VecImpl && );
    /**/                VecImpl       ();
    /**/               ~VecImpl       ();

    VecImpl&            operator=     ( const VecImpl & );
    VecImpl&            operator=     ( VecImpl && );

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
    const Item*         end           () const { return begin() + size(); }
    Item*               end           () { return begin() + size(); }

    CtInt<static_size>  size          ( PI d ) const { return {}; }
    CtInt<static_size>  size          () const { return {}; }

    static constexpr PI nbch          = static_size * sizeof( Item );
    char                data_         [ nbch ]; ///<
};

// dynamic size, items fully on the heap
template<class Item,int alignment>
class VecImpl<Item,-1,0,alignment,true> : public WithDefaultOperators {
public:
    /**/           VecImpl     ( FromSizeAndInitFunctionOnIndex, PI size, auto &&func );
    /**/           VecImpl     ( FromOperationOnItemsOf, auto &&a, auto &&operation );
    /**/           VecImpl     ( FromSizeAndItemValue, PI size, auto &&...ctor_args );
    /**/           VecImpl     ( FromReservationSize, PI capa, PI raw_size = 0 );
    /**/           VecImpl     ( FromItemValues, auto &&...values );
    /**/           VecImpl     ( const HasSizeAndAccess auto &l );
    /**/           VecImpl     ( const VecImpl & );
    /**/           VecImpl     ( VecImpl && );
    /**/           VecImpl     ();
    /**/          ~VecImpl     ();

    static VecImpl range       ( Item end );

    VecImpl&       operator=   ( const VecImpl &that );
    VecImpl&       operator=   ( VecImpl &&that );

    VecImpl&       operator<<  ( auto &&value ) { push_back( FORWARD( value) ); return *this; }

    const Item&    operator[]  ( PI index ) const;
    Item&          operator[]  ( PI index );
    PI             size_tot    () const { return size(); }
    const Item*    begin       () const { return data(); }
    Item*          begin       () { return data(); }
    const Item*    data        ( PI index ) const;
    Item*          data        ( PI index );
    const Item*    data        () const;
    Item*          data        ();
    const Item*    end         () const { return begin() + size(); }
    Item*          end         () { return begin() + size(); }

    bool           empty       () const { return size_ == 0; }
    PI             size        ( PI d ) const { return size(); }
    PI             size        () const;

    void           push_back_br( auto&&...args ); ///< push_back with Item{ FORWARD( args )... }
    void           push_back   ( auto&&...args ); ///< push_back with Item( FORWARD( args )... )
    void           reserve     ( PI capa );
    void           resize      ( PI size, auto&&...ctor_args );
    void           append      ( auto &&that );
    void           clear       ();

    void           copy_data_to( void *data ) const;

    static Item*   allocate    ( PI nb_items );

    Item*          data_;    ///<
    PI             size_;    ///<
    PI             capa_;    ///<
};

// --------------------------------------------------------------------------------------------------------------------------------------------
#define DTP template<class Item,int static_size,int local_size,int alignment,bool allow_heap>
#define UTP VecImpl<Item,static_size,local_size,alignment,allow_heap>

DTP auto get_compilation_flags( CompilationFlags &cn, CtType<UTP> ) { cn.add_inc_file( "vfs/containers/VecImpl.h" ); }
DTP void for_each_template_arg( CtType<UTP>, auto &&f ) { f( CtType<Item>() ); f( CtInt<static_size>() ); f( CtInt<local_size>() ); f( CtInt<alignment>() ); f( CtInt<allow_heap>() ); }
DTP auto template_type_name( CtType<UTP> ) { return "VecImpl"; }
DTP auto block_types_of( CtType<UTP> ) { return CtTypeList<VecImpl<UTP,1>>(); }
DTP auto ct_sizes_of( CtType<UTP> ) { return CtIntList<static_size>(); }
DTP auto memory_of( const UTP &a ) { return Memory_Cpu(); }

DTP constexpr auto tensor_order( CtType<UTP> ) { return CtInt<1>(); }
DTP constexpr auto item_type( CtType<UTP> ) { return CtType<Item>(); }

Ti auto VecImplType_for( auto item_type, CtInt<i> ) { return CtType<VecImpl<typename GET_DT_VALUE( item_type ),i>>(); }
auto VecImplType_for( auto item_type, PI ) { return CtType<VecImpl<typename GET_DT_VALUE( item_type )>>(); }

#undef DTP
#undef UTP

}

#include "VecImpl.tcc" // IWYU pragma: export

#include "make_ArrayImpl_from_unary_operations.h" // IWYU pragma: export
#include "make_ArrayImpl_from_binary_operations.h" // IWYU pragma: export
