#pragma once

#include <vfs/support/default_operators.h>
#include <vfs/support/CompilationFlags.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>

BEG_VFS_NAMESPACE

///
template<class Item,class CtSizes,int dim_offset=0>
class NdArrayWrapper : public WithDefaultOperators {
public:
    /**/                 NdArrayWrapper       ( PyArrayObject *py_array_object, PI data_offset = 0, bool owned = true );
    /**/                 NdArrayWrapper       ( const NdArrayWrapper & ) = delete;
    /**/                 NdArrayWrapper       ( NdArrayWrapper && ) = delete;
    /**/                ~NdArrayWrapper       ();

    static void          for_each_template_arg( auto &&f ) { f( CtType<Item>() ); f( CtType<CtSizes>() ); f( CtInt<dim_offset>() ); }
    static void          get_compilation_flags( CompilationFlags &cn );
    static auto          template_type_name   ();
    static constexpr int nb_dims              () { return CtSizes::size(); }

    auto                 make_array           ( auto type, auto tensor_order, auto movable, auto &&...ctor_arguments );

    const Item&          operator[]           ( PI index ) const requires ( nb_dims() == 1 );
    auto                 operator[]           ( PI index ) const requires ( nb_dims() > 1 );
    Item&                operator[]           ( PI index ) requires ( nb_dims() == 1 );
    auto                 operator[]           ( PI index ) requires ( nb_dims() > 1 );
    DisplayItem*         display              ( Displayer &ds ) const;
    PI                   stride               ( int dim ) const;
    Ti auto              size                 ( CtInt<i> ) const;
    PI                   size                 ( int dim ) const;
    PI                   size                 () const;
    const Item*          data                 ( PI offset_in_bytes = 0 ) const;
    Item*                data                 ( PI offset_in_bytes = 0 );

    PyArrayObject*       py_array_object;     ///<
    PI                   data_offset;         ///< in bytes
    bool                 owned;               ///<
};

TUVi constexpr auto tensor_order( CtType<NdArrayWrapper<U,V,i>> ) { return CtInt<V::size()>(); }
//
END_VFS_NAMESPACE

#include "NdArrayWrapper.tcc" // IWYU pragma: export
