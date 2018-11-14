
#include "ssc.hpp"
#include "logger.hpp"

namespace anthems {
tcp_client::tcp_client() {
    m_serv =std::make_shared<asio_s_raw>();

}
ss_conn tcp_client::connect(const std::string &host, const std::string &port) {

    auto sock = ss_conn(*m_serv);
    tcp_r_raw resolver(*m_serv);
    tcp_q query(host, port);
    auto ran = resolver.resolve(query);
    if (ran.size() == 0) {
        anthems::log("unknown address",host,port);
        throw std::logic_error("unknown address");
    }
    //connect the first
    sock->connect(*ran.begin());
    return sock;
}


}