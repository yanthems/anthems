
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


     std::size_t read(anthems::bytes& buf)override;
     std::size_t write(anthems::bytes &buf)override;

    std::tuple<std::string,std::string>parse_addr();

#if 0
    bytes read_all();
    std::size_t write_all(anthems::bytes&data);
#endif
private:
    cipher m_cipher;
};

}

#endif //ANTHEMS_CIPHER_CONN_HPP
