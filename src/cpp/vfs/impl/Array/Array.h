#pragma once

#include "ArrayTagListAnalyzer.h"
#include "EmptyArrayImpl.h"
#include "../../Int.h"

BEG_VFS_NAMESPACE

///
template<class Item,class... Tags>
class Array : public VfsTdWrapper<Array<Item,Tags...>,sizeof(void*)+2*sizeof(PI),alignof(void*)>, WithDefaultOperators {
public:
    STD_METHODS_FOR_VFS_TD_WRAPPER_TA( Array, Item, Tags, "VFS_NAMESPACE", "vfs" );
    using                    Sizes               = Array<Int,ArrayTag::ForceNbDimsTo<1>>;

    // additional ctors to allow construction using {} lists
    TT                       Array               ( const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &values );
    TT                       Array               ( const std::initializer_list<std::initializer_list<T>> &values );
    TT                       Array               ( const std::initializer_list<T> &values );

    /**/                     Array               ( FromSizeAndItemValue, auto &&sizes, auto &&item_value ) {
        using Func = FuncInfo<CtString<"construct_Array_FromSizeAndItemValue">,CtStringList<"inc_file:vfs/impl/Array/construct_Array_FromSizeAndItemValue.h">,CtIntList<0>>;
        vfs_call<void>( Func(), *this, FORWARD( sizes ), FORWARD( item_value ) );
    }

    Sizes                    sizes               () const;
    Int                      size                () const;

    Array&                   operator<<          ( const Item &item ) { VFS_CALL_METHOD( push_back, void, *this, FORWARD( item ) ); return *this; }
    Array&                   operator<<          ( Item &&item ) { VFS_CALL_METHOD( push_back, void, *this, FORWARD( item ) ); return *this; }

    auto                     operator()          ( auto &&...indices ); ///< return a temporary SelectArray instance. `this` is taken as a non-owning pointer
    void                     set_item            ( auto &&value, auto &&...indices ); ///<
    Item                     get_item            ( auto &&...indices ) const ; ///<

    // static ctors
    static Array             fill                ( const Sizes &sizes, const Item &value );

    // tag analysis
    Ti static constexpr bool want_ct_size_for_dim() { return ArrayTagListAnalyzer::want_ct_size_for_dim( CtInt<i>(), Tags()... ); }
    static auto              type_of_select      ( const auto &...indices );
};

// types for ctors --------------------------------------------------------------------------
template<class Item,class... Tags> struct VfsTdImplFor<Array<Item,Tags...>> { using value = EmptyArrayImpl<Array<Item,Tags...>,ArrayTagListAnalyzer::requested_nb_dims( Tags()... )>; };
// template<class Item,class... Tags> struct VfsTdImplFor<Array<Item,Tags...>,FromSizeAndItemValue,class Size> { using value = EmptyArrayImpl<Array<Item,Tags...>,ArrayTagListAnalyzer::requested_nb_dims( Tags()... )>; };

// type info -------------------------------------------------------------------------------------------
TTA auto constexpr tensor_order( CtType<Array<T,A...>> ) { return ArrayTagListAnalyzer::requested_nb_dims( A()... ); }
TTA auto constexpr item_type( CtType<Array<T,A...>> ) { return CtType<T>(); }

END_VFS_NAMESPACE

#include "Array.tcc"
