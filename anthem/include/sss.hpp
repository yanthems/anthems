
#ifndef ANTHEMS_SSS_HPP
#define ANTHEMS_SSS_HPP

#include "ss_conn.hpp"
#include "asio.hpp"

namespace anthems {

class tcp_server {
public:



    tcp_server(const std::string &port,asio::ip::tcp ver=tcpv4);

    ss_conn accept();

private:
    asio_s m_serv;
    tcp_acp m_acp;
};

class udp_server{
public:
    udp_server(const std::string&host,const std::string&port,asio::ip::udp ver=udpv4);

};

}

#endif //ANTHEMS_SSS_HPP
