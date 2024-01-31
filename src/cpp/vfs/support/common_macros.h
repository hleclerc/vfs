#pragma once

#include "vfs_namespace.h" // IWYU pragma: export
#include <utility> // IWYU pragma: export

// shortcuts for template<...>
#define     TTij                          template<class T,int i,int j>
#define     TUVi                          template<class U,class V,int i>
#define     TTA                           template<class T,class... A>
#define     TTI                           template<class T,std::size_t i>
#define     TTi                           template<class T,int i>
#define     TiT                           template<int i,class T>
#define     TUV                           template<class U,class V>
#define     TiA                           template<int i,class... A>
#define     Tij                           template<int i,int j>
#define     TS                            template<CtStringValue S>
#define     TA                            template<class... A>
#define     TT                            template<class T>
#define     TU                            template<class U>
#define     Ti                            template<int i>

// type handling
#define     VALUE_IN_DECAYED_TYPE_OF( v ) std::decay_t<decltype( v )>::value
#define     CT_DECAYED_TYPE_OF( v )       VFS_NAMESPACE::CtType<std::decay_t<decltype( v )>>()
#define     DECAYED_TYPE_OF( v )          std::decay_t<decltype( v )>
#define     FORWARD( v )                  std::forward<decltype( v )>( v )
