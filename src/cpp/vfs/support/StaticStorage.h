#pragma once

namespace Vfs {

/**
 * Used to make singleton that are initialized once at loading time
*/
template<class T,class...>
struct StaticStorage {
    static T value;
};

template<class T,class...A>
T StaticStorage<T,A...>::value;

}
