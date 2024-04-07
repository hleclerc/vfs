#pragma once

#include "../../vfs_system/WrapperTypeFor.h"
#include "../../vfs_system/VfsTdWrapper.h"
#include "../../make_ct_value.h"
#include "VfsTdImpl_StdType.h"

BEG_VFS_NAMESPACE

/// Wrap an type
class Type : public VfsTdWrapper<Type,sizeof(PI64),alignof(PI64)> {
public:
    STD_METHODS_FOR_VFS_TD_WRAPPER( Type, "VFS_NAMESPACE", "vfs" );

    /**/ Type( auto &&name, auto &&compilation_flags, auto &&template_parameters ) {
        vfs_call<void>( FuncInfo<CtString<"construct_Type">,CtStringList<"inc_file:vfs/impl/Type/construct_Type.h">>(), make_ct_value( name ), make_ct_value( compilation_flags ), make_ct_value( template_parameters ) );
    }
};

// types for ctors
TT struct VfsTdImplFor<Type,CtType<T>> { using value = VfsTdImpl_StdType<Type,T>; };
template<> struct VfsTdImplFor<Type> { using value = VfsTdImpl_StdType<Type,void>; };

// value to wrapper
TT struct VfsWrapperTypeFor<CtType<T>> { using value = Type; };

END_VFS_NAMESPACE

