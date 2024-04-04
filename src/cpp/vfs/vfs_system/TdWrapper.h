#pragma once

#include "../support/operators/ceil.h" // IWYU pragma: export
#include "../vfs_system/VfsFunc.h" // IWYU pragma: export
#include "../support/call_new.h" // IWYU pragma: export
#include "TdKey.h"

BEG_VFS_INTERNAL_NAMESPACE

///
template<class FinalWrapper,int size,int alig>
class alignas( alig ) TdWrapper {
public:
    static constexpr PI ds          = ceil( sizeof( TdKey<FinalWrapper> ), alig ) + size - sizeof( TdKey<FinalWrapper> );

    auto                wrapper_keys() const { return tie( type_info ); }

    TdKey<FinalWrapper> type_info;  ///<
    char                data[ ds ]; ///<
};

///


// macros ------------------------------------------------------------------------------------------------------------------------------
#define STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, INCLUDE_PATH ) public: \
    /**/                   NAME                      ( FromTypeAndCtorArguments, auto &&type, auto &&...args ) { VFS_CALL_DINK( call_new, void, CtStringList<>(), CtIntList<0>(), *this, FORWARD( type ), FORWARD( args )... ); } \
    \
    static void            get_compilation_flags     ( auto &cn ) { cn.add_inc_file( INCLUDE_PATH "/" #NAME ".h" ); } \
    DisplayItem*           display                   ( auto &ds ) const { return vfs_call<DisplayItem *>( FuncInfo<CtString<"display">>(), ds, *this ); } \


#define STD_METHODS_FOR_VFS_TD_WRAPPER( NAME, NAMESPACE, PATH) \
    static auto            type_name                 () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, PATH )

#define STD_METHODS_FOR_VFS_TD_WRAPPER_TT( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); f( CtType<TEMPLATE_ARG_1>() ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, PATH, SIZE, ALIG )

#define STD_METHODS_FOR_VFS_TD_WRAPPER_TA( NAME, TEMPLATE_ARG_0, TEMPLATE_ARG_1, NAMESPACE, PATH, SIZE, ALIG ) \
    static void            for_each_template_arg     ( auto &&f ) { f( CtType<TEMPLATE_ARG_0>() ); ( f( CtType<TEMPLATE_ARG_1>() ), ... ); } \
    static auto            template_type_name        () { return NAMESPACE "::" #NAME; } \
    \
    STD_METHODS_FOR_VFS_TD_WRAPPER__BASE( NAME, PATH, SIZE, ALIG )


END_VFS_INTERNAL_NAMESPACE
