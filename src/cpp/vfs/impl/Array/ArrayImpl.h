#pragma once

#include "../../vfs_system/VfsTdImpl.h"
#include "../../support/Tup.h"

BEG_VFS_NAMESPACE

/// nb_dim = -1 => undefined
///
/// Array wrapper
///
template<class Wrapper            ,
         class Item               ,
         class Capa               = Tup<PI>,
         class Sizes              = Tup<PI>,
         class Strides            = Tup<CtInt<sizeof(Item)>>,
         int   alignment_in_bytes = 0,
         bool  need_row_alignment = true,
         bool  owned              = true
        >
class ArrayImpl : public VfsTdImpl<Wrapper,ArrayImpl<Wrapper,Item,Capa,Sizes,Strides,alignment_in_bytes,need_row_alignment,owned>> {
public:
    /**/                  ArrayImpl            ( FromReservationSize, auto &&sizes, auto &&capa, auto &&strides );
    /**/                  ArrayImpl            ( FromReservationSize, auto &&sizes, auto &&capa );
    /**/                  ArrayImpl            ( FromReservationSize, auto &&sizes );
    /**/                  ArrayImpl            ( FromShapeAndItemValue, auto &&sizes, auto &&item );
    /**/                  ArrayImpl            ( FromShapeAndIterator, auto &&sizes, auto iterator );
    /**/                  ArrayImpl            ( FromShapeAndValues, auto &&sizes, auto &&...values );
    /**/                  ArrayImpl            ( FromAttributes, auto &&strides, auto &&sizes, auto &&capa, char *data );
    /**/                  ArrayImpl            ( const ArrayImpl &that ) = delete;
    /**/                  ArrayImpl            ( ArrayImpl &&that );
    /**/                 ~ArrayImpl            ();

    void                  operator=            ( const ArrayImpl &that ) = delete;
    void                  operator=            ( ArrayImpl &&that ) = delete;

    auto                  operator[]           ( PI index ) const;
    auto                  offset               ( auto &&...indices ) const;
    void                  set                  ( PI index, auto &&value );

    static void           get_compilation_flags( CompilationFlags &cn );
    static void           for_each_template_arg( auto &&f );
    static auto           template_type_name   ();
    void                  for_each_indices     ( auto &&func ) const;
    static auto           tight_strides        ( const auto &capa );
    // void               for_each_item        ( const auto &func );
    auto                  reserve_for          ( auto &&wanted_capa, auto &&func_on_new_array );
    void                  push_back            ( auto &&value );
    constexpr static auto nb_dims              ();
    constexpr auto        size                 () const;


    NUA Strides           strides;
    NUA Sizes             sizes;
    NUA Capa              capa;
    NUA char*             data;
};

END_VFS_NAMESPACE

#include "ArrayImpl.tcc"
