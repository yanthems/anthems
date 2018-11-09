#include "cipher_stream.hpp"
#include <random>
#include <limits>
namespace anthems {

bytes aes_cipher::xor_stream(const bytes&src, DecOrEnc doe) {
    //AES_cfb128_encrypt
    auto res = bytes(src.size());
    int pos = 0;
    switch (doe) {
    case anthems::cipher_stream::DecOrEnc::Decrypt:
        AES_cfb128_encrypt(&src[0], &res[0], src.size(), &enc, &m_iv[0], &pos, AES_ENCRYPT);
        break;
    case anthems::cipher_stream::DecOrEnc::Encrypt:
        AES_cfb128_encrypt(&src[0], &res[0], src.size(), &enc, &m_iv[0], &pos, AES_DECRYPT);
        break;
    default:
        break;
    }
    return res;
}
bytes aes_cipher::encrypt(const bytes& src){
    return xor_stream(src, DecOrEnc::Encrypt);
}
bytes aes_cipher::decrypt(const bytes& src) {
    return xor_stream(src, DecOrEnc::Decrypt);
}

bytes aes_cipher::init_encrypt() {
    //初始化加密
    if (!AES_set_encrypt_key(&m_key[0], keyLen*8, &enc)) {
        throw(std::string("init aes encrypt failed!"));
    }
    //构造随机向量
    std::random_device rd;
    std::default_random_engine dre(rd());
    std::uniform_int_distribution<unsigned char>ud(
        std::numeric_limits<unsigned char>::min(),
        std::numeric_limits<unsigned char>::max());
    auto res = bytes(ivLen);
    for (auto i = 0; i < ivLen; i++) {
        res[i] = ud(dre);
    }
    m_iv = res;
    return res;
}

void aes_cipher::init_decrypt(const bytes& iv) {
    m_iv = iv;
    if (!AES_set_decrypt_key(&m_key[0], keyLen*8, &dec)) {
        throw(std::string("init aes decrypt failed!"));
    }
}

cipher_stream* aes_cipher::copy() {
    auto ptr = new aes_cipher(*this);
    return dynamic_cast<cipher_stream*>(ptr);
}
}