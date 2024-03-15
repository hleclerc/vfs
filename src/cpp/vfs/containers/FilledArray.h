#pragma once

#include "../support/CompilationFlags.h"
#include "Vec.h"

BEG_VFS_NAMESPACE

///
template<class Item,int dim>
class FilledArray {
public:
    static auto  get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/FilledArray.h" ); }
    static void  for_each_template_arg( auto &&f ) { f( CtType<Item>() ); f( CtInt<dim>() ); }
    static auto  template_type_name   () { return "FilledArray"; }

    PI           size                 () const { return sizes[ 0 ]; }

    const auto&  operator()           ( PI i, auto... j ) const { return operator[]( i )( j... ); }
    const auto&  operator()           ( PI i ) const { return operator[]( i ); }
    const auto&  operator[]           ( PI i ) const;

    Item         get_item             ( const auto &...indices ) const { return operator()( indices... ); }
    TUV void     set_item             ( CtType<U> array_type, CtType<V> item_type, auto &&value, const auto &...indices ) {
        if ( this->value != value ) {
            using Dst = VALUE_IN_DECAYED_TYPE_OF( vfs_dt_impl_type( item_type, FORWARD( value ) ) );
            throw typename U::template TypeException<Vec<Dst>,FilledArray>{ std::move( *this ) };
        }
    }

    Vec<PI,dim>  sizes;
    Item         value;
};

#define DTP template<class Item,int dim>
#define UTP FilledArray<Item,dim>

DTP const auto &FilledArray<Item, dim>::operator[]( PI i ) const {
    if constexpr ( dim > 1 )
        return *reinterpret_cast<const FilledArray<Item,dim-1> *>( &sizes[ 1 ] );
    else
        return value;
}

DTP constexpr auto &item_sample( const UTP &that, CtInt<-1> ) {
    return that.value;
}

template<class ArrayType,class Item,int dim>
ArrayType make_FilledArray( CtType<ArrayType>, auto &&sizes, Item &&value, CtInt<dim> ) {
    return { FromValue{}, FilledArray<std::decay_t<Item>,dim>{ FORWARD( sizes ), FORWARD( value ) } };
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
