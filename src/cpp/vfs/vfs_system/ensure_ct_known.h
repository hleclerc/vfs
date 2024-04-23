#pragma once

#include "../support/string/va_string.h"
#include "../support/CtStringList.h"
#include "VfsGenericKey.h"
#include "VfsWrapper.h"

BEG_VFS_NAMESPACE

///
template<CtStringValue struct_name,class Value>
class EnsureCtKnown : public VfsWrapper {
public:
    using       Key                          = VfsGenericKey<Value>;

    static void for_each_template_arg        ( auto &&f ) { f( CtString<struct_name>() ); f( CtType<Value>() ); }
    static auto template_type_name           () { return "EnsureCtKnown"; }

    void        wrapper_get_compilation_flags( CompilationFlags &cg ) const { }
    Str         wrapper_cast_type            () const { return va_string( "$0<$1>", struct_name, key.value ); }
    auto        wrapper_keys                 () const { return tie( key ); }

    Key         key;                         ///<
};

auto ensure_ct_known( auto &&value ) requires std::is_integral_v<std::decay_t<decltype(value)>> {
    return EnsureCtKnown<"CtInt",STORAGE_TYPE_OF( value )>{ .key = { FORWARD( value ) } };
}

TSs auto ensure_ct_known( CtStringList<S...> value ) { return value; }
TA  auto ensure_ct_known( CtTypeList<A...>   value ) { return value; }
Tis auto ensure_ct_known( CtIntList<i...>    value ) { return value; }
TS  auto ensure_ct_known( CtString<S>        value ) { return value; }
TT  auto ensure_ct_known( CtType<T>          value ) { return value; }
Ti  auto ensure_ct_known( CtInt<i>           value ) { return value; }

END_VFS_NAMESPACE
