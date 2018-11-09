#ifndef ANTHEMS_CIPHER_STREAM_HPP
#define ANTHEMS_CIPHER_STREAM_HPP

#include "bytes.hpp"
#include <openssl/aes.h>

namespace anthems {

//interface
struct cipher_interface {
    virtual bytes init_encrypt() = 0;
    virtual void init_decrypt(const bytes& iv) = 0;
    virtual bytes encrypt(const bytes&src) = 0;
    virtual bytes decrypt(const bytes&src) = 0;
};
struct cipher_stream :public cipher_interface{
    enum struct DecOrEnc {
        Decrypt = 1,
        Encrypt = 2,
    };
    bytes m_key;
    bytes m_iv;
    cipher_stream(int keyl,int ivl):keyLen(keyl),ivLen(ivl){}
    virtual cipher_stream* copy() = 0;
    virtual ~cipher_stream(){}
    int keyLen;
    int ivLen;
};

//AES_cfb128_encrypt
class aes_cipher :public cipher_stream {
    using super = cipher_stream;
public:
    aes_cipher(int keyl,int ivl):super(keyl,ivl){}
    virtual ~aes_cipher(){}
    bytes init_encrypt()override;
    void init_decrypt(const bytes& iv)override;
    bytes encrypt(const bytes&src)override;
    bytes decrypt(const bytes&src) override;
    cipher_stream* copy()override;
private:
    bytes xor_stream(const bytes&src, DecOrEnc doe);
private:
    AES_KEY enc;
    AES_KEY dec;
};

}

#endif //ANTHEMS_CIPHER_STREAM_HPP