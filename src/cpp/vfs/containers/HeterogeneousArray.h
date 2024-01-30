#pragma once

#include "../support/CompilationFlags.h"

BEG_VFS_NAMESPACE

/// sequence of items that can be of any type in CtTypeListOfItems
template<class CtTypeListOfItems>
struct HeterogeneousArray {
    /**/           ~HeterogeneousArray   () { for_each_item( [&]( auto &item ) { delete &item; } ); }

    static void     get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/containers/HeterogeneousArray.h" ); }
    static void     for_each_template_arg( auto &&f ) { f( CtType<CtTypeListOfItems>() ); }
    static auto     template_type_name   () { return "HeterogeneousArray"; }
    void            for_each_item        ( auto &&func ) const;
    void            for_each_item        ( auto &&func );
    DisplayItem*    display              ( Displayer &ds ) const;
    static CtInt<1> nb_dims              () { return {}; }
    PI              size                 () const { return type_numbers.size(); }
    
    Vec<PI>         type_numbers;
    Vec<void *>     pointers;
};

#define DTP template<class CtTypeListOfItems>
#define UTP HeterogeneousArray<CtTypeListOfItems>

DTP void UTP::for_each_item( auto &&func ) const {
    for( PI i = 0; i < type_numbers.size(); ++i ) {
        CtTypeListOfItems::get_type( type_numbers[ i ], [&]( auto ct_type ) {
            func( *reinterpret_cast<const typename VALUE_IN_DECAYED_TYPE_OF( ct_type ) *>( pointers[ i ] ) );
        } );
    }
}

DTP void UTP::for_each_item( auto &&func ) {
    for( PI i = 0; i < type_numbers.size(); ++i ) {
        CtTypeListOfItems::get_type( type_numbers[ i ], [&]( auto ct_type ) {
            func( *reinterpret_cast<typename VALUE_IN_DECAYED_TYPE_OF( ct_type ) *>( pointers[ i ] ) );
        } );
    }
}

DTP DisplayItem *UTP::display( Displayer &ds ) const {
    std::vector<DisplayItem *> items;
    for_each_item( [&]( const auto &item ) {
        items.push_back( VFS_NAMESPACE::display( ds, item ) );
    } );
    return ds.array( std::move( items ) );
}

DTP auto ct_value( const UTP &array ) {
    if constexpr ( CtTypeListOfItems::size() == 1 ) {
        if constexpr ( StrLike<typename CtTypeListOfItems::Head> ) {
            RtStringList res;
            array.for_each_item( [&]( const auto &item ) {
                res.value.push_back( item );
            } );
            return res;
        } else
            STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( 0, "..." );
    } else
        STATIC_ASSERT_WITH_RETURN_IN_IF_CONSTEXPR( 0, "..." );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
