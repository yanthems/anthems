
#include "ssc.hpp"
#include "logger.hpp"

namespace anthems {
tcp_client::tcp_client() {
    m_serv =std::make_shared<asio_s_raw>();
    m_solver=std::make_shared<tcp_r_raw>(*m_serv);
}
tcp_client::~tcp_client() {
}

ss_conn tcp_client::connect(const anthems::tcp_q &query) {
    auto ran = m_solver->resolve(query);
    int count = 0;
    auto sock = ss_conn(*m_serv);
    bool isConn = false;
    for (auto &i:ran) {
        anthems::Debug("address =", count++, i.endpoint().address(), i.endpoint().port());
        //选择ipv4
//        if (i.endpoint().protocol() == tcpv4) {
            //connect the first
            sock->connect(i);
            isConn = true;
            break;
//        }
    }
    if (isConn) {
        return sock;
    }
    throw std::domain_error("can't find address");
}

ss_conn tcp_client::connect(const std::string &host, const std::string &port) {
    tcp_q query(host, port);
    return connect(query);

}
ss_conn tcp_client::connect(const std::string &host, std::size_t port) {
    tcp_q query(host,std::to_string(port));
    return connect(query);
}
ss_conn tcp_client::connect(const std::string &addr){
    tcp_q query(addr);
    return connect(query);
}


}