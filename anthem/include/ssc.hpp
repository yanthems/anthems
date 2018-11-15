#ifndef ANTHEMS_SSC_HPP
#define ANTHEMS_SSC_HPP

#include "bytes.hpp"

#include "ss_conn.hpp"

namespace anthems {

class tcp_client {
public:

    tcp_client();

    ss_conn connect(const std::string &host, const std::string &port);
    ss_conn connect(const std::string&addr);
private:
    asio_s m_serv;
    ss_conn connect(const tcp_q&query);
};

//todo udp
class udp_client{
public:

};

}

#endif //ANTHEMS_SSC_HPP
