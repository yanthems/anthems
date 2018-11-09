
#ifndef ANTHEMS_CIPHER_HPP
#define ANTHEMS_CIPHER_HPP
#include <string>

#include "bytes.hpp"
#include "cipher_stream.hpp"

namespace anthems {

class cipher {
private:
    
public:
    explicit cipher(const std::string& method, const std::string& password);
    virtual ~cipher();
    cipher(const cipher&other);
    cipher operator=(const cipher&other);
    cipher copy();
    static bytes evpBytesToKey(std::string password, int kenLen);
    static bytes md5Sum(const bytes& ori);
    static bytes md5Sum(bytes&&ori);
    static bytes md5Sum(const std::string& str);
    static bytes md5Sum(std::string&&str);

public:

private:
    cipher() {};
    cipher_stream *method=nullptr;
};

}
#endif //ANTHEMS_CIPHER_HPP