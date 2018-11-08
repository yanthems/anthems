
#include "test.hpp"


#include <openssl/aes.h>
#include <openssl/md5.h>
#include <map>
#include <cstring>
namespace anthems {


bytes::bytes(const std::string&str)
    :super(str.length()) {
    std::size_t index = 0;
    for (const auto&i : str) {
        this->at(index++)= i;
    }
}
bytes::bytes(const char*str) 
:super(std::strlen(str)){
    for (unsigned int index=0; *str != '\0'; str++) {
        this->at(index++) = *str;
    }
}
bytes::bytes(std::size_t t)
    :super(t) {
}
bytes& bytes::operator+=(const bytes&data) {
    auto start = this->size();
    this->resize(this->size() + data.size());
    for (auto i = 0; i < data.size(); i++) {
        this->at(start + i) = data[i];
    }
    return *this;
}

cipher::cipher(std::string method,std::string password) {
    //AES_cfb128_encrypt()
}

bytes cipher::evpBytesToKey(std::string password, int kenLen) {
    constexpr const int md5Len = 16;

    auto cnt = (kenLen - 1) / md5Len + 1;
    auto m = bytes(cnt*md5Len);

}
bytes cipher::md5Sum(const bytes& ori) {
    auto res = bytes(16);
    MD5(&ori[0], ori.size(), &res[0]);
    return res;
}

}