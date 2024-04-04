#pragma once

#include "KeyWithIndexAndArraySize.h"
#include "../support/TypeConfig.h"
#include "../support/Tuple.h"
// #include <map>

BEG_VFS_INTERNAL_NAMESPACE

///
template<class Callable,class TupleOfKeys>
class VfsFuncArray;

/// 1 indexable arg
template<class Callable,IsAKeyWithIndexAndArraySize Key>
class VfsFuncArray<Callable,Tuple<Key>> {
public:
    /**/       VfsFuncArray( Callable *init );
    /**/      ~VfsFuncArray();

    Callable** operator()  ( const auto &tuple_of_keys );

    PI         size;
    Callable** ptr;
};

// // 0 arg
// template<class Callable>
// class VfsFuncArray<Callable,0> {
// public:
    // /**/       VfsFuncArray( Callable *init, CtType<Tuple<>> );

    // Callable** operator()  ();

    // Callable*  ptr;
// };

// // 1 arg
// template<class Callable>
// class VfsFuncArray<Callable,1> {
// public:
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

END_VFS_INTERNAL_NAMESPACE

#include "VfsFuncArray.tcc" // IWYU pragma: export

