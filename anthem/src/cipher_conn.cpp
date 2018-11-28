
#include "cipher_conn.hpp"
#include "logger.hpp"
#include "protocol.hpp"
namespace anthems{


cipher_conn::cipher_conn(anthems::ss_conn c, const std::string &method, const std::string &password)
:super(std::forward<ss_conn>(c)),
 m_cipher(method,password){

}

cipher_conn::cipher_conn(anthems::ss_conn con, anthems::cipher &&cip)
:super (std::forward<ss_conn>(con)),
m_cipher(std::forward<cipher>(cip)){

}

cipher_conn::cipher_conn(asio::io_service &io, anthems::cipher &&cip)
:super(io),m_cipher(std::forward<cipher>(cip)){

}

std::size_t cipher_conn::read( anthems::bytes&buf) {
    auto iv=super::read_all(m_cipher.method->ivLen);
    if(!m_cipher.method->is_init_decrypt()){
        m_cipher.method->init_decrypt(iv);
    }
    auto len=super::read(buf);
    buf.resize(len);
    buf=m_cipher.method->decrypt(buf);
    return len;
}
std::size_t cipher_conn::write( anthems::bytes &buf) {
    if(!m_cipher.method->is_init_encrypt()){
        m_cipher.method->init_encrypt();
    }
    auto iv=m_cipher.method->enc_iv;
    auto data=m_cipher.method->encrypt(buf);
    auto res=iv+data;
    super::write_all(res);
    return res.size();
}

std::tuple<std::string,std::string> cipher_conn::parse_addr() {
//    auto data=anthems::sockv5::do_parse(*this);
    auto data=anthems::bytes(anthems::sockv5::ReqMax);
    auto len=this->read(data);
    return anthems::sockv5::parse_addr(data);
}

#if 0
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
#endif
}
