#pragma once

#include "../../vfs_system/VfsTdImpl.h"
#include "../../support/Tup.h"
#include "ArrayImplData.h"

BEG_VFS_NAMESPACE

/// nb_dim = -1 => undefined
///
/// Array wrapper
///
template<class Wrapper            ,
         class Item               ,
         class Capa               = Tup<PI>,
         class Shape              = Tup<PI>,
         class Strides            = Tup<CtInt<sizeof(Item)>>,
         int   alignment_in_bytes = 0,
         bool  need_row_alignment = true,
         bool  owned              = true
        >
class ArrayImpl : public VfsTdImpl<Wrapper,ArrayImpl<Wrapper,Item,Capa,Shape,Strides,alignment_in_bytes,need_row_alignment,owned>,ArrayImplData<Strides, Shape, Capa>> {
public:
    /**/                  ArrayImpl            ( const ArrayImpl &that ) = default;
    /**/                  ArrayImpl            ( ArrayImpl &&that ) = default;
    /**/                  ArrayImpl            ( auto&&...args );
    /**/                 ~ArrayImpl            ();

    void                  operator=            ( const ArrayImpl &that ) = default;
    void                  operator=            ( ArrayImpl &&that ) = default;

    auto                  operator[]           ( PI index ) const; ///< item at pos index according to the first dimension
    auto                  offset               ( auto &&...indices ) const;
    void                  set                  ( PI index, auto &&value );

    static void           get_compilation_flags( CompilationFlags &cn );
    static void           for_each_template_arg( auto &&f );
    static auto           template_type_name   ();
    auto                  reserve_for          ( auto &&wanted_capa, auto &&func_on_new_array );
    void                  push_back            ( auto &&value );
    constexpr static auto nb_dims              ();
    constexpr auto        size                 () const; ///< nb items in the first dimension
};

END_VFS_NAMESPACE

#include "ArrayImpl.tcc"
