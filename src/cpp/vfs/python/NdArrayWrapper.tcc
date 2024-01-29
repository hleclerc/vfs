#pragma once

#include <vfs/containers/Vec.h>
#include "NdArrayWrapper.h"

BEG_VFS_NAMESPACE

#define DTP template<class Item,class CtSizes,int dim_offset>
#define UTP NdArrayWrapper<Item,CtSizes,dim_offset>

DTP UTP::NdArrayWrapper( PyArrayObject *py_array_object, PI data_offset, bool owned ) : py_array_object( py_array_object ), data_offset( data_offset ), owned( owned ) {
    if ( owned )
        Py_INCREF( py_array_object );
}

DTP UTP::~NdArrayWrapper() {
    if ( owned )
        Py_DECREF( py_array_object );
}

DTP void UTP::get_compilation_flags( CompilationFlags &cn ) {
    cn.add_inc_file( "vfs/python/NdArrayWrapper.h" );
    cn.add_inc_path( "`python3-config --includes`" );
}

DTP auto UTP::template_type_name() {
    return "NdArrayWrapper";
}

DTP auto UTP::make_array( auto type, auto tensor_order, auto movable, auto &&...ctor_arguments ) {
    TODO;
    // using TR = GenericVector<typename GET_DT_VALUE( type ),Memory_Cpu,-1,0,0,1>;
    // return TR( Memory_Cpu(), FORWARD( ctor_arguments )... );
}

DTP const Item &UTP::operator[]( PI index ) const requires ( nb_dims() == 1 ) {
    return *data( stride( 0 ) * index );
}

DTP auto UTP::operator[]( PI index ) const requires ( nb_dims() > 1 ) {
    return NdArrayWrapper<Item,typename CtSizes::Tail,dim_offset+1>( py_array_object, stride( 0 ) * index, false );
}

DTP Item &UTP::operator[]( PI index ) requires ( nb_dims() == 1 ) {
    return *data( stride( 0 ) * index );
}

DTP auto UTP::operator[]( PI index ) requires ( nb_dims() > 1 ) {
    return NdArrayWrapper<Item,typename CtSizes::Tail,dim_offset+1>( py_array_object, stride( 0 ) * index, false );
}

DTP DisplayItem *UTP::display( Displayer &ds ) const {
    Vec<DisplayItem *> v;
    v.reserve( size( 0 ) );
    for( PI i = 0; i < size( 0 ); ++i )
        v.push_back( VFS_NAMESPACE::display( ds, operator[]( i ) ) );
    return ds.array( v );
}

DTP PI UTP::stride( int dim ) const {
    return PyArray_STRIDE( py_array_object, dim_offset + dim );
}

DTP Ti auto UTP::size( CtInt<i> ci ) const {
    if constexpr ( CtSizes::get( ci ) >= 0 )
        return CtSizes::get( ci );
    else
        return PyArray_DIM( py_array_object, dim_offset + i );
}

DTP PI UTP::size( int dim ) const {
    return PyArray_DIM( py_array_object, dim_offset + dim );
}

DTP PI UTP::size() const {
    // return ct_range<nb_dims()>().reduction( [&]( auto d, auto v ) { return v * size( d ); }, CtInt<1>{} );
    return size( CtInt<0>() );
}

DTP const Item *UTP::data( PI offset_in_bytes ) const {
    return reinterpret_cast<const Item *>( PyArray_BYTES( py_array_object ) + data_offset + offset_in_bytes );
}

DTP Item *UTP::data( PI offset_in_bytes ) {
    return reinterpret_cast<Item *>( PyArray_BYTES( py_array_object ) + data_offset + offset_in_bytes );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
