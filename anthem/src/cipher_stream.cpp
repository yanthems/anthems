#include "cipher_stream.hpp"
#include <random>
#include <limits>
#include "logger.hpp"
namespace anthems {

aes_cipher::aes_cipher(const bytes &keys, int ivl)
        : super(keys, ivl) {
    auto block_size = m_key.size() * 8;
    if (AES_set_encrypt_key(m_key.data(), block_size, &enc) != 0) {
        throw (std::logic_error("init aes encrypt failed!"));
    }
//    if (AES_set_decrypt_key(m_key.data(), keyLen * 8, &dec)!=0) {
//        throw (std::logic_error("init aes decrypt failed!"));
//    }

//    log("the key = ",m_key);
}

//function call will change the dec_iv or env_iv every time
bytes aes_cipher::xor_stream(const bytes &src, DecOrEnc doe) {
    //AES_cfb128_encrypt
    auto res = bytes(src.size());
    int pos = 0;
    switch (doe) {
        case DecOrEnc::Decrypt: {
            AES_cfb128_encrypt(src.data(), res.data(), src.size(),
                               &enc, dec_iv.data(), &pos, AES_DECRYPT);
            break;
        }
        case DecOrEnc::Encrypt: {
            AES_cfb128_encrypt(src.data(), res.data(), src.size(),
                               &enc, enc_iv.data(), &pos, AES_ENCRYPT);
            break;
        }
    }
    return res;
}

//encrypt the src bytes
bytes aes_cipher::encrypt(const bytes &src) {
    return xor_stream(src, DecOrEnc::Encrypt);
}

//decrypt the src bytes
bytes aes_cipher::decrypt(const bytes &src) {
    return xor_stream(src, DecOrEnc::Decrypt);
}

//init the enc_iv for are_cfv_128
bytes aes_cipher::init_encrypt() {
    std::random_device rd;
    std::default_random_engine dre(rd());
    std::uniform_int_distribution<unsigned char> ud(
            std::numeric_limits<unsigned char>::min(),
            std::numeric_limits<unsigned char>::max());
    auto res = bytes(ivLen);
    for (auto i = 0; i < ivLen; i++) {
        res[i] = ud(dre);
    }
    enc_iv = res;
    return res;
}

//copy the dec_iv for are_cfv_128
void aes_cipher::init_decrypt(const bytes &iv) {
    dec_iv = iv;
}

cipher_stream *aes_cipher::copy() {
    auto ptr = new aes_cipher(*this);
    return dynamic_cast<cipher_stream *>(ptr);
}


}