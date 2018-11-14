
#include "sss.hpp"
#include <future>

#include "bytes.hpp"
#include "logger.hpp"
#include "ss_conn.hpp"

namespace anthems {

tcp_server::tcp_server(const std::string &port,asio::ip::tcp ver) {

    m_serv = std::make_shared<asio::io_service>();
    auto uiport = std::stoi(port);
    auto m_ep = tcp_ep_raw(ver, uiport);
    m_acp = std::make_shared<tcp_acp_raw>(*m_serv, m_ep);

}


ss_conn tcp_server::accept() {
    auto conn = ss_conn(*m_serv);
    m_acp->accept(*conn);
    return conn;
}

udp_server::udp_server(const std::string &host, const std::string &port, asio::ip::udp ver) {

    auto m_serv=std::make_shared<asio::io_service>();
    auto uiport = std::stoi(port);

    auto m_ep=asio::ip::udp::endpoint(ver,uiport);

    asio::ip::udp::resolver res(*m_serv);

    auto rt=res.resolve(m_ep);


}

}