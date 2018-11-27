
#ifndef ANTHEMS_LOGGER_HPP
#define ANTHEMS_LOGGER_HPP

#include <iostream>
#include "helper.hpp"

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

#if LOG_LEVEL >=3
static inline constexpr int LogLevel=3;
#else
#if LOG_LEVEL>=2
static inline constexpr int LogLevel=2;
#else
static inline constexpr int LogLevel=1;
#endif
#endif

static inline const int DebugLevel=3;
static inline const int WarningLevel=2;
static inline const int ErrorLevel=1;

template<typename ...Args>
inline void Debug(Args &&... args) {
    if constexpr(LogLevel>=DebugLevel) {
        anthems::log("[DEBUG]",std::forward<Args>(args)...);
    }
}
template<typename ...Args>
inline void Warning(Args &&... args) {
    if constexpr(LogLevel>=WarningLevel) {
        anthems::log("[WARNING]",std::forward<Args>(args)...);
    }
}
template<typename ...Args>
inline void Error(Args &&... args) {
    if constexpr(LogLevel>=ErrorLevel) {
        anthems::log("[ERROR]",std::forward<Args>(args)...);
    }
}

}
#endif //ANTHEMS_LOGGER_HPP
