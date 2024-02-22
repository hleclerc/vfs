#pragma once

#include "../containers/Vec.h"

BEG_VFS_NAMESPACE
class CompilationFlags;

/**
*/
class VfsDtTypeAncestor {
public:
    /**/                      VfsDtTypeAncestor          ();

    virtual PI32              get_instantiated_type_index() = 0;
    virtual void              get_compilation_flags_rec  ( CompilationFlags &res, Vec<Str> &seen ) const = 0;
    virtual int               nb_indirections            () const = 0;
    virtual Vec<Str>          final_types                () const = 0;
    virtual Vec<Str>          final_refs                 () const = 0;
    virtual Str               cast_type                  () const = 0;
    virtual Str               cast_ref                   () const;
    virtual DisplayItem*      display                    ( Displayer &ds ) const = 0;

    static void               register_the_new_types     ();
    static VfsDtTypeAncestor* type_at_global_index       ( PI32 index );

    PI32                      global_type_index          = 0; ///< in `global_vector`
    VfsDtTypeAncestor*        prev_new_type;             ///<
};

END_VFS_NAMESPACE
