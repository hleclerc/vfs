#pragma once

#include "../containers/Vec.h"

BEG_VFS_NAMESPACE
class CompilationFlags;

/**
*/
class VfsTdTypeAncestor {
public:
    /**/                      VfsTdTypeAncestor          ();

    virtual PI32              get_instantiated_type_index() = 0;
    virtual void              get_compilation_flags_rec  ( CompilationFlags &res, Vec<Str> &seen ) const = 0;
    virtual DisplayItem*      display                    ( Displayer &ds ) const = 0;
    virtual Str               name                       () const = 0;

    static void               register_the_new_types     ();
    static VfsTdTypeAncestor* type_at_global_index       ( PI32 index );

    PI32                      global_type_index          = 0; ///< in `global_vector`
    VfsTdTypeAncestor*        prev_new_type;             ///<
};

END_VFS_NAMESPACE
