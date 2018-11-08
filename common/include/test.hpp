
#ifndef ANTHEMS_CIPHER_HPP
#define ANTHEMS_CIPHER_HPP
#include <string>
#include <iostream>
#include <utility>
#include <vector>

namespace anthems {

class bytes :public std::vector<unsigned char> {
    using super = std::vector<unsigned char>;
public:
    bytes(const std::string&str);
    bytes(const char*str);
    bytes(std::size_t t);
    bytes& operator+=(const bytes&data);
};

template<typename T>
inline void log(T&& t) {
    std::cout << std::forward<T>(t) << std::endl;
}
template<typename T, typename ...Args>
inline void log(T&& t, Args&&... args) {
    std::cout << std::forward<T>(t) << ' ';
    anthems::log(std::forward<Args>(args)...);
}

class cipher {
private:
    enum struct DecOrEnc {
        Decrypt =1,
        Encrypt =2,
    };
public:
    explicit cipher(std::string method, std::string password);
    static bytes evpBytesToKey(std::string password, int kenLen);
    static bytes md5Sum(const bytes& ori);
};

}
#endif //ANTHEMS_CIPHER_HPP