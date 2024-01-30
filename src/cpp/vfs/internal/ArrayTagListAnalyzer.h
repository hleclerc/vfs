#pragma once

//#include "../containers/Memory/Memory_Cpu.h"
#include "../containers/EmptyArrayImpl.h"
#include "ArrayTagList.h"

BEG_VFS_NAMESPACE

// function to analyze array tags ----------------------------------------------------------------------------------------------------------------------------------------------
namespace ArrayTagListAnalyzer {

auto array_type_for_ctor_args( auto sub_obj_type, auto tags, const HasSizeAndAccess auto &v );

// sub_array_tag_list
TiA constexpr auto sub_array_tag_list          ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> );
TTA constexpr auto sub_array_tag_list          ( ArrayTagList<T,A...> );
constexpr auto     sub_array_tag_list          ( ArrayTagList<> );

TiA constexpr auto sub_array_tag_list          ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> ) { return ArrayTagList<ArrayTag::ForceNbDimsTo<i-1>,decltype( sub_array_tag_list( ArrayTagList<A...>() ) )>(); }
TTA constexpr auto sub_array_tag_list          ( ArrayTagList<T,A...> ) { return ArrayTagList<T,decltype( sub_array_tag_list( ArrayTagList<A...>() ) )>(); }
constexpr auto     sub_array_tag_list          ( ArrayTagList<> ) { return ArrayTagList<>(); }

// want_ct_size_in
TA constexpr auto  want_ct_size_in             ( ArrayTagList<ArrayTag::WantCtSizeInAllDims,A...>, auto dim );
TTA constexpr auto want_ct_size_in             ( ArrayTagList<T,A...>, auto dim );
constexpr auto     want_ct_size_in             ( ArrayTagList<>, auto dim );

TA constexpr auto  want_ct_size_in             ( ArrayTagList<ArrayTag::WantCtSizeInAllDims,A...>, auto dim ) { return CtInt<1>(); }
TTA constexpr auto want_ct_size_in             ( ArrayTagList<T,A...>, auto dim ) { return want_ct_size_in( ArrayTagList<A...>(), dim ); }
constexpr auto     want_ct_size_in             ( ArrayTagList<>, auto dim ) { return CtInt<0>(); }

// want_nb_dims
TiA constexpr auto want_nb_dims                ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> );
TTA constexpr auto want_nb_dims                ( ArrayTagList<T,A...> );
constexpr auto     want_nb_dims                ( ArrayTagList<> );

TiA constexpr auto want_nb_dims                ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> ) { return CtInt<i>(); }
TTA constexpr auto want_nb_dims                ( ArrayTagList<T,A...> ) { return want_nb_dims( ArrayTagList<A...>() ); }
constexpr auto     want_nb_dims                ( ArrayTagList<> ) { return CtInt<-1>(); }

//
auto               array_type_for_type_and_size( auto sub_obj_type, auto tags, auto item_type, auto &&size ) {
    // runtime size
    if constexpr ( want_nb_dims( tags ) == -1 ) {
        // find nb dims in ctor args
        return item_type;
    } else

    // scalar
    if constexpr ( want_nb_dims( tags ) == 0 ) {
        return item_type;
    } else

    // vector
    if constexpr ( want_nb_dims( tags ) == 1 ) {
        // template<class Item,int static_size=-1,int local_size=0,int alignment=0,bool allow_heap=true>
        if constexpr ( want_ct_size_in( tags, CtInt<0>() ) )
            return Type( "Vec", "inc_file:vfs/containers/Vec.h", sub_item_type, size, CtInt<0>(), CtInt<0>(), CtInt<0>() );
        else

        //
        return Type( "Vec", "inc_file:vfs/containers/Vec.h", ctor_item_type, CtInt<-1>(), CtInt<0>(), CtInt<0>(), CtInt<1>() );

    } else

    // size >= 2
    // auto sub_item_type = array_type_for( item_type( ctor_item_type ), sub_array_tag_list( tags ), size );
    STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( , "TODO" );
}

// array_type_for =>
auto             array_type_for_ctor_args    ( auto sub_obj_type, auto tags, const HasSizeAndAccess auto &v ) {
    if constexpr( requires { v.begin(); } )
        return ArrayTagListAnalyzer::array_type_for_type_and_size( sub_obj_type, tags, DECAYED_CT_OF( *v.begin() ), v.size() );
    else
        return ArrayTagListAnalyzer::array_type_for_type_and_size( sub_obj_type, tags, DECAYED_CT_OF( v[ 0 ] ), v.size() );
}

// // real type of array if constructed from...
// auto array_type_for_ctor_args( auto tags, auto item_type, const HasSizeAndAccess auto &v ) {
// }

// // default ArrayType
// auto array_type_for_ctor_args( auto tags, auto item_type ) {
//     return CtType<EmptyArrayImpl>();
// }


// helper concept to check if there is an array_type_for_ctor_args
template<class Item,class Tags,class...Args>
concept Has_array_type_for_ctor_args = requires ( Tags tags, const Args &...args ) {
    array_type_for( CtType<Item>(), tags, args... );
};

} // namespace ArrayTagListAnalyzer

END_VFS_NAMESPACE
