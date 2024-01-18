#pragma once

#include "../support/TypeConfig.h"
#include <map>

namespace Vfs {

///
template<class KeyList,class Callable>
class VfsFuncArray;

// generic case
template<class... KeyTypes,class Callable>
class VfsFuncArray<CtTypeList<KeyTypes...>,Callable> {
public:
    using      Key         = std::tuple<KeyTypes...>;
    using      FuncMap     = std::map<Key,Callable *>;

    /**/       VfsFuncArray( Callable *init );
    Callable** operator()  ( auto &&...keys );

    FuncMap    func_map;
    Callable*  init;
};

// /// 0 arg
// template<class Callable>
// class VfsFuncArray<Callable,0,false> {
// public:
//     TA            VfsFuncArray( Callable *init, S<A>... );

//     TA Callable** operator()  ( const A &... );

//     Callable*     ptr;
// };

// /// 1 arg
// template<class Callable>
// class VfsFuncArray<Callable,1,false> {
// public:
//     /**/          VfsFuncArray( const VfsFuncArray & ) = delete;
//     TA            VfsFuncArray( Callable *init, S<A>... );
//     /**/         ~VfsFuncArray();

//     TA Callable** operator()  ( const A &... args );

//     PI            size;
//     Callable**    ptr;
// };

// /// 2 args
// template<class Callable>
// class VfsFuncArray<Callable,2,false> {
// public:
//     /**/          VfsFuncArray( const VfsFuncArray & ) = delete;
//     TA            VfsFuncArray( Callable *init, S<A>... );
//     /**/         ~VfsFuncArray();

//     TA Callable** operator()  ( const A &... args );

//     PI            size_a;
//     PI            size_b;
//     Callable**    ptr;
// };

// /// n args
// template<class Callable,int nb_vfs_args>
// class VfsFuncArray<Callable,nb_vfs_args,false> {
// public:
//     using         Key         = std::vector<PI32>; ///< vfs_type_crea
//     using         Map         = std::map<Key,Callable *>;

//     TA            VfsFuncArray( Callable *init, S<A>... );

//     TA Callable** operator()  ( const A &... );

//     Callable*     init;       ///<
//     Map           map;        ///<
// };

// /// with parameters
// template<class Callable,int nb_vfs_args>
// class VfsFuncArray<Callable,nb_vfs_args,true> {
// public:
//     using         TargetFunc  = VfsFuncArray<Callable,nb_vfs_args,false>;
//     using         FuncMap     = std::map<Str,std::unique_ptr<TargetFunc>>;

//     TA            VfsFuncArray( Callable *init, S<A>... );

//     TA Callable** operator()  ( const A &... args );

//     FuncMap       func_map;
//     Callable*     init;
// };

}

#include "VfsFuncArray.tcc" // IWYU pragma: export

