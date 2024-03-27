#pragma once

#include "../support/operators/ceil.h" // IWYU pragma: export
#include "../support/TypeConfig.h" // IWYU pragma: export

BEG_VFS_NAMESPACE

///
template<PI size_,PI alig_>
struct alignas( alig_ ) VfsWrapperAttributes {
    static constexpr PI    act_size                = ceil( 2 * sizeof( PI32 ), alig_ ) - 2 * sizeof( PI32 );
    static constexpr PI    min_size                = size_;
    static constexpr PI    alig                    = alig_;

    TUV void               construct               ( CtType<U>, FromTypeAndCtorArguments, CtType<V>, auto &&...args );

    mutable PI32           instantiated_type_index;
    PI32                   global_type_index;
    char                   data[ act_size ];
};

// impl ------------------------------------------------------------------------------------------------------
#define DTP template<PI size_,PI alig_>
#define UTP VfsWrapperAttributes<size_,alig_>

DTP TUV void UTP::construct( CtType<U>, FromTypeAndCtorArguments, CtType<V>, auto &&...args ) {
    new ( this ) V( FORWARD( args )... );
}

#undef DTP
#undef UTP

END_VFS_NAMESPACE
