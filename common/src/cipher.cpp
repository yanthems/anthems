
#include "cipher.hpp"

#include <openssl/md5.h>
#include <map>
#include <cstring>
#include <functional>
namespace anthems {

static std::map<std::string, cipher_stream*>cipherMethod = {
    {
        "aes-128-cfb",
        new aes_cipher(16,16),
    },
    {
        "aes-192-cfb",
        new aes_cipher(24,16),
    },
    {
        "aes-256-cfb",
        new aes_cipher(32,16),
    },
};

cipher::cipher(const std::string& method, const std::string& password) {
    if (method == "") {
        throw(std::string("method is null!"));
    }
    if (password == "") {
        throw(std::string("password is null!"));
    }
    
    this->method = dynamic_cast<cipher_stream*>(cipherMethod[method]);
    if (this->method == nullptr) {
        throw(std::string("this method is not allowed!"));
    }
    this->method->m_key = evpBytesToKey(password, this->method->keyLen);
}
cipher::~cipher() {
    delete method;
}

cipher::cipher(const cipher & other) {
    return other.copy();
}

cipher cipher::operator=(const cipher & other) {
    return other.copy();
}

 cipher cipher::copy() {
    cipher res;
    res.method = this->method->copy();
    return res;
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
bytes cipher::md5Sum(const bytes& ori) {
    auto res = bytes(16);
    MD5(&ori[0], ori.size(), &res[0]);
    return res;
}
bytes cipher::md5Sum(bytes&&ori) {
    auto res = bytes(16);
    MD5(&ori[0], ori.size(), &res[0]);
    return res;
}

bytes cipher::md5Sum(const std::string& str) {
    auto res = bytes(16);
    return md5Sum(bytes(str));
}
bytes cipher::md5Sum(std::string&&str) {
    return md5Sum(bytes(std::forward<std::string>(str)));
}


}