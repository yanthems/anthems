
#ifndef ANTHEMS_CIPHER_CONN_HPP
#define ANTHEMS_CIPHER_CONN_HPP

#include "ss_conn.hpp"
#include "cipher.hpp"

namespace anthems{

class cipher_conn:public ss_conn{
    using super=ss_conn;

public:
    cipher_conn(ss_conn&&c,const std::string&method,const std::string&password);
    cipher_conn(ss_conn&&con,cipher&& cip);
    cipher_conn(asio::io_service &io,cipher&&cip);
    bytes read(std::size_t) override;
    std::size_t write(const anthems::bytes&data) override ;

//    bytes read(std::size_t size,asio::error_code& err)override ;
//    std::size_t write(const anthems::bytes &data, asio::error_code &err)override ;

    bytes read_all();
    std::size_t write_all(anthems::bytes&data);
private:
    cipher m_cipher;
};

}

#endif //ANTHEMS_CIPHER_CONN_HPP
