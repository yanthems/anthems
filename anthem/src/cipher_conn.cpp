
#include "cipher_conn.hpp"
#include "logger.hpp"
namespace anthems{


cipher_conn::cipher_conn(anthems::ss_conn &&c, const std::string &method, const std::string &password)
:super(std::forward<ss_conn>(c)),
 m_cipher(method,password){

}

cipher_conn::cipher_conn(anthems::ss_conn &&con, anthems::cipher &&cip)
:super (std::forward<ss_conn>(con)),
m_cipher(std::forward<cipher>(cip)){

}

cipher_conn::cipher_conn(asio::io_service &io, anthems::cipher &&cip)
:super(io),m_cipher(std::forward<cipher>(cip)){

}

//anthems::cipher_conn::Block
std::size_t cipher_conn::write(const bytes&data) {
    if(!m_cipher.method->is_init_encrypt()){
        m_cipher.method->init_encrypt();
    }
    auto iv=m_cipher.method->enc_iv;
    auto edata=m_cipher.method->encrypt(data);
    auto res=iv+edata;
//    log("iv ===",iv);
//    log("data ===",edata);
//    log("write ===>",res);
    return this->super::write(res);
}
bytes cipher_conn::read(std::size_t size) {
    auto iv=this->super::read(m_cipher.method->ivLen);
    if(!m_cipher.method->is_init_decrypt()){
        m_cipher.method->init_decrypt(iv);
    }
    auto ddata=this->super::read(size);
    auto res=m_cipher.method->decrypt(ddata);
//    log("iv ===",iv);
//    log("data ===",ddata);
//    log("read raw msg===>",res);
    return res;
}

bytes cipher_conn::read_all() {
    auto res=anthems::bytes();
    try {
        while (true){
            res+=cipher_conn::read(super::Block);
        }
    }catch (const std::exception&e){
        anthems::log(e.what());
    }
    return res;
}
std::size_t cipher_conn::write_all(anthems::bytes &data) {
    std::size_t res=0;
    try {
        std::size_t begin=0;
        while (true){
            auto some=data.split(begin,begin+super::Block);
            if(some.empty()){
                break;
            }
            begin+=super::Block;
            res+=cipher_conn::write(some);
        }
    }catch (const std::exception&e){
        anthems::log(e.what());
    }
    return res;
}

}