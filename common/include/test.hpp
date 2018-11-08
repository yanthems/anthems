
#ifndef ANTHEMS_CIPHER_HPP
#define ANTHEMS_CIPHER_HPP
#include <string>
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

using bytes = std::vector<unsigned char>;

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