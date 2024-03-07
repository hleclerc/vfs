#include "VfsDtTypeAncestor.h"

BEG_VFS_NAMESPACE

static Vec<VfsDtTypeAncestor *> all_the_types( FromSize(), 1024 );
static VfsDtTypeAncestor *last_new_type = nullptr;
PI32 VfsDtTypeAncestor::total_nb_types = 0;

VfsDtTypeAncestor::VfsDtTypeAncestor() {
    global_type_index = total_nb_types++;
    prev_new_type = last_new_type;
    last_new_type = this;
}

void VfsDtTypeAncestor::register_the_new_types() {
    for( ; last_new_type; last_new_type = last_new_type->prev_new_type ) {
        auto p = last_new_type->global_type_index;
        while( p >= all_the_types.size() )
            all_the_types.resize( std::max( 2 * all_the_types.size(), 1ul ) );
        all_the_types[ p ] = last_new_type;
    }
}

VfsDtTypeAncestor *VfsDtTypeAncestor::type_at_global_index( PI32 index ) {
    register_the_new_types();
    ASSERT( index < all_the_types.size() );
    return all_the_types[ index ];
}

END_VFS_NAMESPACE
