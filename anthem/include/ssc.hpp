#ifndef ANTHEMS_SSC_HPP
#define ANTHEMS_SSC_HPP

#include "bytes.hpp"

#include "ss_conn.hpp"

namespace anthems {

class tcp_client {
public:

    tcp_client();

    ss_conn connect(const std::string &host, const std::string &port);
    ss_conn connect(const std::string &host,std::size_t port);
    ss_conn connect(const std::string&addr);
    ss_conn connect(const tcp_q&query);
private:
    asio_s m_serv;
};

//todo udp
class udp_client{
public:

};

}

#endif //ANTHEMS_SSC_HPP
