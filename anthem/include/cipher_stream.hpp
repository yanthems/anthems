#ifndef ANTHEMS_CIPHER_STREAM_HPP
#define ANTHEMS_CIPHER_STREAM_HPP

#include "bytes.hpp"
#include <openssl/aes.h>
#include <memory>

namespace anthems {

//interface
struct cipher_interface {
    virtual bytes init_encrypt() = 0;

    virtual void init_decrypt(const bytes &iv) = 0;

    virtual bytes encrypt(const bytes &src) = 0;

    virtual bytes decrypt(const bytes &src) = 0;
};

struct cipher_stream : public cipher_interface {
    enum struct DecOrEnc {
        Decrypt = 1,
        Encrypt = 2,
    };
    bytes m_key;
    bytes enc_iv;
    bytes dec_iv;

    cipher_stream(const bytes&keys,int ivl) :m_key(keys),ivLen(ivl){}
    virtual cipher_stream* copy()=0;
    virtual void reset(){
        enc_iv=bytes();
        dec_iv=bytes();
    }
    virtual bool is_init_encrypt(){
        return !enc_iv.empty();
    }
    virtual bool is_init_decrypt(){
        return !dec_iv.empty();
    }

    virtual ~cipher_stream() {}

    std::size_t ivLen;
};

//AES_cfb128_encrypt
class aes_cipher : public cipher_stream {
    using super = cipher_stream;
public:
    aes_cipher(const bytes&keys,int ivl) ;

    virtual ~aes_cipher() {}

    bytes init_encrypt() override;

    cipher_stream* copy()override;


    void init_decrypt(const bytes &iv) override;

    bytes encrypt(const bytes &src) override;

    bytes decrypt(const bytes &src) override;

private:
    bytes xor_stream(const bytes &src, DecOrEnc doe);

private:
    AES_KEY enc;
//    AES_KEY dec;
};

}

#endif //ANTHEMS_CIPHER_STREAM_HPP