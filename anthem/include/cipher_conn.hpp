
#ifndef ANTHEMS_CIPHER_CONN_HPP
#define ANTHEMS_CIPHER_CONN_HPP

#include "ss_conn.hpp"
#include "cipher.hpp"

namespace anthems{

class cipher_conn{

public:
    static const constexpr auto Block=bytes::Large_Block;
public:
    cipher_conn(ss_conn&&c,const std::string&method,const std::string&password);

    bytes read(std::size_t size=Block);
    void write(const bytes&data);
private:
    cipher m_cipher;
    ss_conn m_conn;
};

}

#endif //ANTHEMS_CIPHER_CONN_HPP
