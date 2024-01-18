#include "VfsTdTypeAncestor.h"

namespace Vfs {

static Vec<VfsTdTypeAncestor *> all_the_types( 1024 );
static VfsTdTypeAncestor *last_new_type = nullptr;
static PI total_nb_types = 0;

VfsTdTypeAncestor::VfsTdTypeAncestor() {
    global_type_index = total_nb_types++;
    prev_new_type = last_new_type;
    last_new_type = this;
}

void VfsTdTypeAncestor::register_the_new_types() {
    for( ; last_new_type; last_new_type = last_new_type->prev_new_type ) {
        auto p = last_new_type->global_type_index;
        while( p >= all_the_types.size() )
            all_the_types.resize( std::max( 2 * all_the_types.size(), 1ul ) );
        all_the_types[ p ] = last_new_type;
    }
}

VfsTdTypeAncestor *VfsTdTypeAncestor::type_at_global_index( PI32 index ) {
    register_the_new_types();
    ASSERT( index < all_the_types.size() );
    return all_the_types[ index ];
}

}
