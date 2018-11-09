#ifndef ANTHEMS_BYTES_HPP
#define ANTHEMS_BYTES_HPP

#include <iostream>
#include <utility>
#include <vector>

namespace anthems {

template<typename T>
inline void log(T&& t) {
    std::cout << std::forward<T>(t) << std::endl;
}
template<typename T, typename ...Args>
inline void log(T&& t, Args&&... args) {
    std::cout << std::forward<T>(t) << ' ';
    anthems::log(std::forward<Args>(args)...);
}

using byte = unsigned char;
const constexpr std::size_t ToEnd = std::size_t(-1);

class bytes :public std::vector<byte> {
    using super = std::vector<byte>;
public:
    bytes(const std::string&str);
    bytes(const char*str);
    bytes(std::size_t t);
    bytes();
    bytes& operator+=(const bytes&data);
    bytes& operator+=(const char*str);
    bytes& operator+=(const std::string&str);
    friend bytes operator+(const bytes&src, const bytes&data);
    friend bytes operator+(bytes&&src, bytes&&data);
    friend bytes operator+(const bytes&src, const char*str);
    friend bytes operator+(const bytes&src, const std::string&str);
    bytes split(std::size_t begin, std::size_t end = ToEnd);
};

}
#endif // !ANTHEMS_BYTES_HPP
