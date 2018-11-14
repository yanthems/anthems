
#include "cipher_conn.hpp"
#include "logger.hpp"
namespace anthems{


cipher_conn::cipher_conn(anthems::ss_conn &&c, const std::string &method, const std::string &password)
:m_conn(std::forward<ss_conn>(c)),m_cipher(method,password){

}


//anthems::cipher_conn::Block
void cipher_conn::write(const bytes&data) {

    if(!m_cipher.method->is_init_encrypt()){
        m_cipher.method->init_encrypt();
    }
    auto iv=m_cipher.method->enc_iv;
    auto edata=m_cipher.method->encrypt(data);
    auto res=iv+edata;

//    log("iv ===",iv);
//    log("data ===",edata);
//    log("write ===>",res);

    m_conn.write(res);

}
bytes cipher_conn::read(std::size_t size) {
    auto iv=m_conn.read(m_cipher.method->ivLen);
    if(!m_cipher.method->is_init_decrypt()){
        m_cipher.method->init_decrypt(iv);
    }
    auto ddata=m_conn.read(size);
    auto res=m_cipher.method->decrypt(ddata);
//    log("iv ===",iv);
//    log("data ===",ddata);
//    log("read raw msg===>",res);
    return res;
}

}