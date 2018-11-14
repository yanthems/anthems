
#ifndef ANTHEMS_LOGGER_HPP
#define ANTHEMS_LOGGER_HPP

#include <iostream>
namespace anthems {

template<typename T>
inline void log(T &&t) {
    std::cout << std::forward<T>(t) << std::endl;
}

template<typename T, typename ...Args>
inline void log(T &&t, Args &&... args) {
    std::cout << std::forward<T>(t) << ' ';
    anthems::log(std::forward<Args>(args)...);
}

}
#endif //ANTHEMS_LOGGER_HPP
