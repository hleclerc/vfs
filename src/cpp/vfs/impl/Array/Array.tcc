#pragma once

//#include "support/with_sizes_and_sample_item_ptr.h"
//#include "containers/SelectArray.h"
#include "Array.h"
//#include "Any.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item,class... Tags>
#define UTP Array<Item,Tags...>

// DTP TT UTP::Array( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values ) : Array( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<Array>(), values ), values ) {
// }

// DTP TT UTP::Array( const std::initializer_list<std::initializer_list<T>> &values ) : Array( FromTypeAndCtorArguments(), vfs_dt_impl_type( CtType<Array>(), values ), values ) {
// }

DTP TT UTP::Array( const std::initializer_list<T> &values ) : Array( FromSizeAndIterator(), values.size(), values.begin() ) {
}

DTP UTP::Array( FromSizesAndItemValue, auto &&sizes, auto &&item_value ) {
    using Func = FuncInfo<CtString<"construct_Array_FromSizesAndItemValue">,CtStringList<"inc_file:vfs/impl/Array/construct_Array_FromSizesAndItemValue.h">,CtIntList<0>>;
    vfs_call<void>( Func(), *this, FORWARD( sizes ), FORWARD( item_value ) );
}

DTP UTP::Array( FromSizesAndIterator, auto &&sizes, auto &&iterator ) {
    using Func = FuncInfo<CtString<"construct_Array_FromSizesAndIterator">,CtStringList<"inc_file:vfs/impl/Array/construct_Array_FromSizesAndIterator.h">,CtIntList<0>>;
    vfs_call<void>( Func(), *this, FORWARD( sizes ), FORWARD( iterator ) );
}

DTP UTP::Array( FromSizeAndIterator, auto &&size, auto &&iterator ) {
    using Func = FuncInfo<CtString<"construct_Array_FromSizeAndIterator">,CtStringList<"inc_file:vfs/impl/Array/construct_Array_FromSizeAndIterator.h">,CtIntList<0>>;
    vfs_call<void>( Func(), *this, FORWARD( size ), FORWARD( iterator ) );
}

// DTP Int UTP::size() const {
//     return VFS_CALL_METHOD( size, Int, *this );
// }

// DTP auto UTP::operator()( auto &&...indices ) {
//     auto tup = std::make_tuple( FORWARD( indices )... );
//     return SelectArray<Array *,decltype(tup)>{ this, std::move( tup ) };
// }

// DTP void UTP::set_item( auto &&value, auto &&...indices ) {
//     return VFS_CALL_METHOD_WITH_CATCH( set_item, void, *this, CtType<Array>(), CtType<Item>(), FORWARD( value ), FORWARD( indices )... );
// }

// DTP Item UTP::get_item( auto &&...indices ) const {
//     // Pb: le type de sortie n'est pas le même en fonction de la dimension
//     // Prop: on fait ce qu'on peut
//     return VFS_CALL_METHOD( get_item, Item, *this, FORWARD( indices )... );
// }

// DTP UTP UTP::fill( const Sizes &sizes, const Item &value ) {
//     return VFS_CALL_DINK( make_FilledArray, CtStringList<"inc_file:vfs/containers/FilledArray.h">, UTP, CtType<UTP>{}, sizes, value, sizes.size() );
// }

// DTP auto UTP::type_select( const auto &...indices ) {
//     constexpr auto wanted_nb_dims = ArrayTagListAnalyzer::requested_nb_dims( Tags{}... ).value;
//     if constexpr ( sizeof...( indices ) == 0 )
//         return CtType<Array>();
//     else if constexpr ( wanted_nb_dims < 0 )
//         return CtType<Any>();
//     else if constexpr ( sizeof...( indices ) == wanted_nb_dims )
//         return CtType<Item>();
//     else
//         TODO; //return CtType<Array<Item,decltype(ArrayTagListAnalyzer::with_dim_sub( CtInt<sizeof...( indices )>(), Tags{} ))>>();
// }

// DTP auto vfs_dt_impl_type_array_has_size_and_access( CtType<UTP>, const auto &that ) {
//     return with_sizes_and_sample_item_ptr( that, [&]( const auto &sizes, const auto &sample_item_ptr ) {
//         constexpr auto requested_nb_dims = ArrayTagListAnalyzer::requested_nb_dims( Tags{}... );
//         constexpr auto that_nb_dims = sizes.size();
//         if ( requested_nb_dims >= 0 )
//             static_assert( requested_nb_dims == that_nb_dims );

//         if (  ) {

//         }
//         auto ItemStorage = vfs_dt_impl_type( CtType<Item>(), *sample_item_ptr );
//         auto SizesStorage = vfs_dt_impl_type( CtType<Array<Int>>(), sizes );
//         auto StridesStorage = SizesStorage;
//         auto Memory = Memory_Cpu();
//         auto alignment_in_bytes = CtInt<1>();
//         auto need_row_alignment = CtInt<1>();
//         return Type( CtString<"ArrayImpl">(), CtStringList<"vfs/containers/ArrayImpl.h">(), ItemStorage, SizesStorage, StridesStorage, Memory, alignment_in_bytes, need_row_alignment );
//     } );
// }

// DTP auto vfs_dt_impl_type( CtType<UTP>, const HasSizeAndAccess auto &that ) {
//     // call vfs_dt_impl_type_array_has_size_and_access with the exact type + alignment info
//     return VFS_CALL_DINK( vfs_dt_impl_type_array_has_size_and_access, CtStringList<>, Type, CtType<UTP>(), that );

//     // /// If it's empty, we're not going to have sample items...
//     // /// TODO: test if that.size() is constexpr-able
//     // if ( that.size() == 0 ) {
//     //     if ( requested_nb_dims >= 0 )
//     //         return Type( "EmptyArrayImpl", "vfs/containers/EmptyArrayImpl.h", requested_nb_dims );
//     // }

//     // // scalar
//     // auto sub_item_type = vfs_dt_impl_type( CtType<Item>(), item_sample( that, requested_nb_dims ) );
//     // if ( requested_nb_dims == 0 )
//     //     return Type( sub_item_type );

//     // // nb_dims >= 1
//     // return with_sizes( that, [&]( const auto &sizes ) {
//     //     // runtime size
//     //     if ( requested_nb_dims == -1 ) {
//     //         // find nb dims in ctor args
//     //         TODO;
//     //         return Type( sub_item_type );
//     //     }

//     //     // vector
//     //     if ( requested_nb_dims == 1 ) {
//     //         // template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
//     //         // static size
//     //         if ( ArrayTagListAnalyzer::want_ct_size_for_dim( CtInt<0>(), Tags{}... ) )
//     //             return Type( CtString<"Vec">(), CtStringList<"inc_file:vfs/containers/Vec.h">(), sub_item_type, sizes[ 0 ], CtInt<0>(), CtInt<0>(), CtInt<0>() );
//     //         // dynamic size
//     //         return Type( CtString<"Vec">(), CtStringList<"inc_file:vfs/containers/Vec.h">(), sub_item_type, CtInt<-1>(), CtInt<0>(), CtInt<0>(), CtInt<1>() );
//     //     }

//     //     // size >= 2
//     //     TODO;
//     //     return Type( sub_item_type );
//     // } );
// }

// DTP auto vfs_dt_impl_type( CtType<UTP> ) {
//     return CtType<EmptyArrayImpl<-1>>();
// }

#undef DTP
#undef UTP

END_VFS_NAMESPACE
