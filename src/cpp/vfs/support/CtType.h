#pragma once

// #include "IsSpecializationOf.h" // IWYU pragma: export
#include "vfs_namespace.h"
#include <type_traits>

BEG_VFS_NAMESPACE

/// A structure to store a compile time known type
template<class Value>
struct CtType {
    using       value                = Value;

    static void for_each_template_arg( auto &&f ) { f( CtType<Value>() ); }
    static auto template_type_name   () { return "CtType"; }
    static auto to_string            (); ///< defined in type_name.h
    const auto* cast                 ( const auto *v ) const { return reinterpret_cast<const value *>( v ); }
    auto*       cast                 ( auto *v ) const { return reinterpret_cast<value *>( v ); }
};

template<class U>
concept IsA_CtType = std::is_same_v<std::decay_t<U>,CtType<typename std::decay_t<U>::value>>;

/// make an instance of CtType<type of v> if v is known.
auto type_of( const auto &v ) { return VFS_NAMESPACE::CtType<std::decay_t<decltype( v )>>(); }

END_VFS_NAMESPACE
