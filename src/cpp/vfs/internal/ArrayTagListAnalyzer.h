#pragma once

#include "../containers/Vec.h"
#include "../Type.h"
#include "ArrayTagList.h"

BEG_VFS_NAMESPACE

/// struct to analyze array tags ----------------------------------------------------------------------------------------------------------------------------------------------
template<class Item,class... Tags>
struct ArrayTagListAnalyzer;

template<class Item,class HeadTag,class... TailTags>
struct ArrayTagListAnalyzer<Item,HeadTag,TailTags...> {
    static auto requested_nb_dims( auto ) { return ArrayTagListAnalyzer<Item,TailTags...>::requested_nb_dims(); }
    Ti static auto requested_nb_dims( ArrayTag::ForceNbDimsTo<i> ) { return CtInt<i>(); }
    static auto requested_nb_dims() { return requested_nb_dims( HeadTag() ); }
};

template<class Item>
struct ArrayTagListAnalyzer<Item> {
    auto requested_nb_dims() {
        return CtInt<-1>();
    }
};

// auto array_type_for_ctor_args( auto sub_obj_type, auto tags, const HasSizeAndAccess auto &v );

// // sub_array_tag_list
// TiA constexpr auto sub_array_tag_list          ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> );
// TTA constexpr auto sub_array_tag_list          ( ArrayTagList<T,A...> );
// constexpr auto     sub_array_tag_list          ( ArrayTagList<> );

// TiA constexpr auto sub_array_tag_list          ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> ) { return ArrayTagList<ArrayTag::ForceNbDimsTo<i-1>,decltype( sub_array_tag_list( ArrayTagList<A...>() ) )>(); }
// TTA constexpr auto sub_array_tag_list          ( ArrayTagList<T,A...> ) { return ArrayTagList<T,decltype( sub_array_tag_list( ArrayTagList<A...>() ) )>(); }
// constexpr auto     sub_array_tag_list          ( ArrayTagList<> ) { return ArrayTagList<>(); }

// // want_ct_size_for_dim
// TA constexpr auto  want_ct_size_for_dim        ( ArrayTagList<ArrayTag::WantCtSizeInAllDims,A...>, auto dim );
// TTA constexpr auto want_ct_size_for_dim        ( ArrayTagList<T,A...>, auto dim );
// constexpr auto     want_ct_size_for_dim        ( ArrayTagList<>, auto dim );

// TA constexpr auto  want_ct_size_for_dim        ( ArrayTagList<ArrayTag::WantCtSizeInAllDims,A...>, auto dim ) { return CtInt<1>(); }
// TTA constexpr auto want_ct_size_for_dim        ( ArrayTagList<T,A...>, auto dim ) { return want_ct_size_for_dim( ArrayTagList<A...>(), dim ); }
// constexpr auto     want_ct_size_for_dim        ( ArrayTagList<>, auto dim ) { return CtInt<0>(); }

// // want_nb_dims
// TiA constexpr auto want_nb_dims                ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> );
// TTA constexpr auto want_nb_dims                ( ArrayTagList<T,A...> );
// constexpr auto     want_nb_dims                ( ArrayTagList<> );

// TiA constexpr auto want_nb_dims                ( ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...> ) { return CtInt<i>(); }
// TTA constexpr auto want_nb_dims                ( ArrayTagList<T,A...> ) { return want_nb_dims( ArrayTagList<A...>() ); }
// constexpr auto     want_nb_dims                ( ArrayTagList<> ) { return CtInt<-1>(); }

// // with_decreased_dim
// TiA constexpr auto with_dim_sub                ( auto sub, ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...>, auto... new_tags );
// TTA constexpr auto with_dim_sub                ( auto sub, ArrayTagList<T,A...>, auto... new_tags );
// constexpr auto     with_dim_sub                ( auto sub, ArrayTagList<>, auto... new_tags );

// TiA constexpr auto with_dim_sub                ( auto sub, ArrayTagList<ArrayTag::ForceNbDimsTo<i>,A...>, auto... new_tags ) { return with_dim_sub( sub, ArrayTagList<A...>(), ArrayTag::ForceNbDimsTo<i-sub>(), new_tags... ); }
// TTA constexpr auto with_dim_sub                ( auto sub, ArrayTagList<T,A...>, auto... new_tags ) { return with_dim_sub( sub, ArrayTagList<A...>(), T(), new_tags... ); }
// constexpr auto     with_dim_sub                ( auto sub, ArrayTagList<>, auto... new_tags ) { return ArrayTagList<decltype( new_tags )...>(); }

END_VFS_NAMESPACE
