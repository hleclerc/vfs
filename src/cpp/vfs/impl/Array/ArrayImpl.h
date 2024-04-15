#pragma once

#include "../../vfs_system/VfsTdImpl.h"
#include "../../support/Tuple.h"

BEG_VFS_NAMESPACE

/// nb_dim = -1 => undefined
///
/// Array wrapper
///
template<class Wrapper            ,
         class Item               ,
         class Capa               = Tuple<PI>,
         class Sizes              = Tuple<PI>,
         class Strides            = Tuple<CtInt<sizeof(Item)>>,
         int   alignment_in_bytes = 0,
         bool  need_row_alignment = true
         >
class ArrayImpl : public VfsTdImpl<Wrapper,ArrayImpl<Wrapper,Item,Capa,Sizes,Strides,alignment_in_bytes,need_row_alignment>> {
public:
    /**/                  ArrayImpl            ( FromReservationSize, auto &&strides, auto &&sizes, auto &&capa );
    /**/                  ArrayImpl            ( FromSizesAndValues, auto &&sizes, auto &&...values );
    /**/                  ArrayImpl            ( const ArrayImpl &that ) = delete;
    /**/                  ArrayImpl            ( ArrayImpl &&that );
    /**/                 ~ArrayImpl            ();

    void                  operator=            ( const ArrayImpl &that ) = delete;
    void                  operator=            ( ArrayImpl &&that ) = delete;

    static void           get_compilation_flags( CompilationFlags &cn );
    static void           for_each_template_arg( auto &&f );
    static auto           template_type_name   ();
    static auto           tight_strides        ( const Capa &capa );
    void                  for_each_item        ( const auto &func );
    constexpr static auto nb_dims              ();
    constexpr auto        size                 ();

    Strides               strides;
    Sizes                 sizes;
    Capa                  capa;
    Item*                 data;
};

END_VFS_NAMESPACE

#include "ArrayImpl.tcc"
