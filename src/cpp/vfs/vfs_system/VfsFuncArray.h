#pragma once

#include "KeyWithIndexAndArraySize.h"
#include "../support/TypeConfig.h"
#include "../support/Tuple.h"
#include <map>

BEG_VFS_NAMESPACE

/// generic version (std::map)
template<class Callable,class TupleOfKeys>
class VfsFuncArray {
public:
    using      Map         = std::map<TupleOfKeys,Callable *>;

    /**/       VfsFuncArray( Callable *init );

    Callable** operator()  ( auto &&tuple_of_keys );

    Callable*  init;
    Map        map;
};

/// 0 arg
template<class Callable>
class VfsFuncArray<Callable,Tuple<>> {
public:
    /**/       VfsFuncArray( Callable *init );

    Callable** operator()  ( const auto &tuple_of_keys );

    Callable*  ptr;
};

/// 1 indexable arg
template<class Callable,IsAVfsKeyWithIndexAndArraySize Key>
class VfsFuncArray<Callable,Tuple<Key>> {
public:
    /**/       VfsFuncArray( Callable *init );
    /**/      ~VfsFuncArray();

    Callable** operator()  ( const auto &tuple_of_keys );

    PI         size;
    Callable** ptr;
};

/// 2 indexable args
template<class Callable,IsAVfsKeyWithIndexAndArraySize Key_0,IsAVfsKeyWithIndexAndArraySize Key_1>
class VfsFuncArray<Callable,Tuple<Key_0,Key_1>> {
public:
    /**/       VfsFuncArray( Callable *init );
    /**/      ~VfsFuncArray();

    Callable** operator()  ( const auto &tuple_of_keys );

    PI         size_0;
    PI         size_1;
    Callable** ptr;
};

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

END_VFS_NAMESPACE

#include "VfsFuncArray.tcc" // IWYU pragma: export

