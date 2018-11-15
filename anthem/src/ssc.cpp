
#include "ssc.hpp"
#include "logger.hpp"

namespace anthems {
tcp_client::tcp_client() {
    m_serv =std::make_shared<asio_s_raw>();

}

ss_conn tcp_client::connect(const anthems::tcp_q &query) {
    
    anthems::log(query.service_name(),query.host_name());

    tcp_r_raw resolver(*m_serv);
    auto ran = resolver.resolve(query);
    int count=0;
    for(auto &i:ran){
        anthems::log("host:",count++,i.host_name(),i.endpoint().address(),i.endpoint().port());
    }
    if (ran.size() == 0) {
        anthems::log("unknown address",query.service_name());
        throw std::logic_error("unknown address");
    }

    auto sock = ss_conn(*m_serv);
    //connect the first
    sock->connect(*ran.begin());
    return sock;
}
ss_conn tcp_client::connect(const std::string &host, const std::string &port) {
    tcp_q query(host, port);
    return connect(query);

}
ss_conn tcp_client::connect(const std::string &addr){
    tcp_q query(addr);
    return connect(query);
}


}