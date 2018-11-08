
#include "test.hpp"


#include <openssl/aes.h>
#include <openssl/md5.h>
#include <map>

namespace anthems {




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