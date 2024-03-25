#include "VfsTypeAncestor.h"

BEG_VFS_NAMESPACE

static Vec<VfsTypeAncestor *> all_the_types( FromSize(), 1024 );
static VfsTypeAncestor *last_new_type = nullptr;
PI32 VfsTypeAncestor::total_nb_types = 0;

VfsTypeAncestor::VfsTypeAncestor() {
    global_type_index = total_nb_types++;
    prev_new_type = last_new_type;
    last_new_type = this;
}

void VfsTypeAncestor::register_the_new_types() {
    for( ; last_new_type; last_new_type = last_new_type->prev_new_type ) {
        auto p = last_new_type->global_type_index;
        while( p >= all_the_types.size() )
            all_the_types.resize( std::max( 2 * all_the_types.size(), 1ul ) );
        all_the_types[ p ] = last_new_type;
    }
}

VfsTypeAncestor *VfsTypeAncestor::type_at_global_index( PI32 index ) {
    register_the_new_types();
    ASSERT( index < all_the_types.size() );
    return all_the_types[ index ];
}

END_VFS_NAMESPACE
