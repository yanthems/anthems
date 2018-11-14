
#include "cipher.hpp"

#include <openssl/md5.h>
#include <map>
#include <cstring>
#include <functional>
#include <functional>
namespace anthems {

static std::map<std::string,
        std::function< cipher_stream*(const std::string&)> > cipherMethod = {
        {
                "aes-128-cfb",
                [](const std::string& p) {
                    return new aes_cipher(cipher::evpBytesToKey(p,16), 16);
                }
        },
        {
                "aes-192-cfb",
                [](const std::string& p) {
                    return new aes_cipher(cipher::evpBytesToKey(p,24), 16);
                }
        },
        {
                "aes-256-cfb",
                [](const std::string& p) {
                    return new aes_cipher(cipher::evpBytesToKey(p,32), 16);
                }
        },
};

cipher::cipher(const std::string &m, const std::string &p) {
    if (m == "") {
        throw (std::logic_error("method is null!"));
    }
    if (p == "") {
        throw (std::logic_error("password is null!"));
    }
    auto fp=cipherMethod[m];
    if (fp== nullptr){
        throw (std::logic_error("this method is not allowed!"));
    }
    this->method = fp(p);
}

cipher::~cipher() {
    delete method;
}

cipher::cipher(const cipher &other) {
    this->method = this->method->copy();
}

cipher &cipher::operator=(const cipher &other) {
    this->method = this->method->copy();
    return *this;
}

// test
// foobar 32
// 3858F62230AC3C915F300C664312C63F568378529614D22DDB49237D2F60BFDF
// success
bytes cipher::evpBytesToKey(std::string password, int kenLen) {
    constexpr const int md5Len = 16;
    auto cnt = (kenLen - 1) / md5Len + 1;
    auto m = bytes{};
    m += md5Sum(password);
    auto start = 0;
    for (auto i = 0; i < cnt; i++) {
        start += md5Len;
        auto d = m.split(start - md5Len, start);
        d += password;
        m = m.split(0, start) + md5Sum(d);
    }
    return m.split(0, kenLen);
}

bytes cipher::md5Sum(const bytes &ori) {
    auto res = bytes(16);
    MD5(&ori[0], ori.size(), &res[0]);
    return res;
}

bytes cipher::md5Sum(bytes &&ori) {
    auto res = bytes(16);
    MD5(&ori[0], ori.size(), &res[0]);
    return res;
}

bytes cipher::md5Sum(const std::string &str) {
    auto res = bytes(16);
    return md5Sum(bytes(str));
}

bytes cipher::md5Sum(std::string &&str) {
    return md5Sum(bytes(std::forward<std::string>(str)));
}


}