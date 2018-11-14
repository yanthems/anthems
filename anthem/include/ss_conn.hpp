#ifndef ANTHEMS_SSCONN_HPP
#define ANTHEMS_SSCONN_HPP

#include "bytes.hpp"
#include "asio.hpp"

#include <memory>
namespace anthems {

using asio_s_raw=asio::io_service;
using asio_s=std::shared_ptr<asio_s_raw>;

using tcp_ep_raw=asio::ip::tcp::endpoint;
using tcp_ep=std::shared_ptr<tcp_ep_raw>;

using udp_ep_raw=asio::ip::udp::endpoint;
using udp_ep=std::shared_ptr<udp_ep_raw>;

using tcp_acp_raw=asio::ip::tcp::acceptor;
using tcp_acp=std::shared_ptr<tcp_acp_raw>;

using tcp_r_raw=asio::ip::tcp::resolver;
using tcp_r=std::shared_ptr<tcp_r_raw>;

using udp_r_raw=asio::ip::udp::resolver;
using udp_r=std::shared_ptr<udp_r_raw>;

using tcp_q=asio::ip::tcp::resolver::query;
using udp_q=asio::ip::udp::resolver::query;

using asio_socket_raw=asio::ip::tcp::socket;
using asio_socket=std::shared_ptr<asio_socket_raw>;

const auto tcpv4 = asio::ip::tcp::v4();
const auto tcpv6 = asio::ip::tcp::v6();
const auto udpv4 = asio::ip::udp::v4();
const auto udpv6 = asio::ip::udp::v6();

class ss_conn : public asio_socket {
    using super=asio_socket;
public:
    ss_conn(asio::io_service &io);

    anthems::bytes read(std::size_t t);

    anthems::bytes read();

    std::size_t write(const anthems::bytes &data);

    std::size_t write(const char *data);

    std::size_t write(const std::string &data);

private:
    anthems::bytes read_all();
};
}
#endif // !ANTHEMS_SSCONN_HPP
