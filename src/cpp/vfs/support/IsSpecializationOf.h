#pragma once

namespace Vfs {

template<template <typename...> class Template, typename T>
struct IsSpecializationOf{
    static constexpr bool value = 0;
};

template<template <typename...> class Template, typename... Args>
struct IsSpecializationOf<Template,Template<Args...>> {
    static constexpr bool value = 1;
};

template<template <typename...> class Template,class T>
concept is_specialization_of = IsSpecializationOf<Template,T>::value;

// ------------------------------------------------------------------------
template<template <int...> class Template, typename T>
struct IsSpecializationOf_i{
    static constexpr bool value = 0;
};

template<template <int...> class Template, int... Args>
struct IsSpecializationOf_i<Template,Template<Args...>> {
    static constexpr bool value = 1;
};

}
