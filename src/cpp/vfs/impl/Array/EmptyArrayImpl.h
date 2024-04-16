#pragma once

// #include "../../support/CompilationFlags.h"
#include "../../support/StorageTypeFor.h"
#include "../../vfs_system/VfsTdImpl.h"
#include "ArrayImpl.h"

BEG_VFS_NAMESPACE

/// nb_dim = -1 => undefined
template<class Wrapper,int nb_dims_>
class EmptyArrayImpl : public VfsTdImpl<Wrapper,EmptyArrayImpl<Wrapper,nb_dims_>> {
public:
    static void           get_compilation_flags( CompilationFlags &cn ) { cn.add_inc_file( "vfs/impl/Array/EmptyArrayImpl.h" ); }
    static void           for_each_template_arg( auto &&f ) { f( CtType<Wrapper>() ); f( CtInt<nb_dims_>() ); }
    static auto           template_type_name   () { return "VFS_NAMESPACE::EmptyArrayImpl"; }
    void                  for_each_item        ( const auto & ) const {}
    DisplayItem*          display              ( Displayer &ds ) const { return ds.array( {} ); }
    constexpr static auto nb_dims              () { return CtInt<nb_dims_>(); }
    constexpr static auto size                 () { return CtInt<0>(); }

    void                  push_back            ( auto &&value ) {
        using Item = StorageTypeFor<decltype(value)>::value;
        using Strides = Tuple<CtInt<sizeof(Item)>>;
        if constexpr ( Wrapper::template want_ct_size_for_dim<0>() ) {
            using Sizes = Tuple<CtInt<1>>;
            new ( this ) ArrayImpl<Wrapper,Item,Sizes,Sizes,Strides>( FromSizesAndValues(), Sizes{ CtInt<1>{} }, FORWARD( value ) );
        } else {
            using Sizes = Tuple<PI>;
            new ( this ) ArrayImpl<Wrapper,Item,Sizes,Sizes,Strides>( FromSizesAndValues(), Sizes{ PI( 1 ) }, FORWARD( value ) );
        }
    }
};

END_VFS_NAMESPACE
