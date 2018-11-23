#ifndef ANTHEMS_SSCONN_HPP
#define ANTHEMS_SSCONN_HPP

#include "bytes.hpp"
#include "asio.hpp"
#include "logger.hpp"
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

#if __cplusplus>201703L
inline static auto tcpv4 = asio::ip::tcp::v4();
#else
const auto tcpv4=asio::ip::tcp::v4();
#endif
const auto tcpv6 = asio::ip::tcp::v6();
const auto udpv4 = asio::ip::udp::v4();
const auto udpv6 = asio::ip::udp::v6();

class ss_conn : public asio_socket {
    using super=asio_socket;
public:
    static const constexpr auto Block=bytes::Large_Block;
public:
    ss_conn(asio::io_service &io);

    ss_conn(){}
    
    void read_over(){
        anthems::log(__func__);
        (*this)->shutdown((*this)->shutdown_receive);
    }
    void write_over(){
        anthems::log(__func__);
        (*this)->shutdown((*this)->shutdown_send);
    }
    
    virtual anthems::bytes read(std::size_t n);

    virtual std::tuple<anthems::bytes,std::size_t> read_length(std::size_t n);

    virtual anthems::bytes read_enough(std::size_t n);

    virtual std::size_t write(const anthems::bytes &data);

    static std::size_t pipe_then_close(ss_conn &src, ss_conn &dst, const std::string &debug_name = "debug");

#if 0
//    virtual anthems::bytes read(std::size_t n, asio::error_code &err);
//    virtual std::size_t write(const anthems::bytes &data, asio::error_code &err);
//    static std::size_t bib_then_close(ss_conn&src,ss_conn&dst);
#endif
};
}
#endif // !ANTHEMS_SSCONN_HPP
