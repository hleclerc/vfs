#pragma once

#include "../support/CompilationFlags.h"
#include "../support/tuple_from_item.h"
#include "ArrayImpl.h"
#include "../Array.h"

BEG_VFS_NAMESPACE

///
template<class Value,class Sizes>
class FilledArray {
public:
    static auto  get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/FilledArray.h" ); }
    static void  for_each_template_arg( auto &&f ) { f( CtType<Value>() ); f( CtType<Sizes>() ); }
    static auto  template_type_name   () { return "FilledArray"; }

    PI           size                 () const { return sizes[ 0 ]; }

    const auto&  operator()           ( auto... indices ) const { return SelectArray<FilledArray *,DECAYED_TYPE_OF( indices )...>( this, indices... ); }
    const auto&  operator[]           ( PI i ) const { return operator()( i ); }

    auto         get_item             ( const auto &...indices ) const;

    TUV void     set_item             ( CtType<U> array_type, CtType<V> item_type, auto &&value, const auto &...indices ) {
        if ( this->value != value ) {
            using DstItem = VALUE_IN_DECAYED_TYPE_OF( type_promote( CtString<"reassign">(),
                vfs_dt_impl_type( item_type, FORWARD( value ) ), // how to store `value`
                CtType<Value>()
            ) );
            using Dst = ArrayImpl<DstItem,Tuple<>>;
            throw typename U::template TypeException<Dst,FilledArray>{ std::move( *this ) };
        }
    }

    Sizes        sizes;
    Value        value;
};

#define DTP template<class Value,class Sizes>
#define UTP FilledArray<Value,Sizes>

DTP auto UTP::get_item( const auto &...indices ) const {
    if constexpr ( sizeof...( indices ) == std::tuple_size<Sizes>::value )
        return value;
    else {
        auto new_sizes = tuple_from_item( CtInt<sizeof...( indices )>(), sizes );
        return FilledArray<Value,decltype( new_sizes )>( std::move( new_sizes ), value );
    }
}

// DTP TUV void     set_item             ( CtType<U> array_type, CtType<V> item_type, auto &&value, const auto &...indices ) {

// DTP const auto &UTP::operator[]( PI i ) const {
//     if constexpr ( dim > 1 )
//         return *reinterpret_cast<const FilledArray<Item,dim-1> *>( &sizes[ 1 ] );
//     else
//         return value;
// }

DTP constexpr auto &item_sample( const UTP &that, CtInt<-1> ) {
    return that.value;
}

template<class ArrayType,class Item,int dim>
ArrayType make_FilledArray( CtType<ArrayType>, auto &&sizes, Item &&value ) {
    using Sizes = VALUE_IN_DECAYED_TYPE_OF( vfs_dt_impl_type( CtType<Array<Int,ArrayTagList<ArrayTag::ForceNbDimsTo<1>>>>(), sizes ) );
    return { FromValue{}, FilledArray<std::decay_t<Item>,Sizes>{ FORWARD( sizes ), FORWARD( value ) } };
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
