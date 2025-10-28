#ifndef UTIL_H_
#define UTIL_H_

#include <memory>
#include <type_traits>

namespace MusicLib {

namespace Util {

/**
 * @brief Create a copy of the given object, encased in a smart pointer of
 * a given type. A supplement to the clone method some classes have that 
 * allows to get a ponter that isn't at the base of the inheritance chain.
 * 
 * @tparam T Type of the unique pointer to be returned.
 * @tparam B The base from which T ultimately derived (the type which the
 * clone method would return). Usually deduced from T.
 * @param obj 
 * @return std::unique_ptr<T> 
 */
template<typename T, typename B>
std::unique_ptr<T> clone(const B& obj)
{
    static_assert(std::is_base_of_v<B, T>, "class T must be derived from class B");
    return std::unique_ptr<T>(static_cast<T*>(obj.clone().release()));
}

}

}

#endif // UTIL_H_