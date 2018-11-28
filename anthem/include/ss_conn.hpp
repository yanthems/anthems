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

using error=std::error_code;

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
    static const constexpr auto Block=bytes::Block_Size;
public:
    explicit ss_conn(asio::io_service &io);
    ss_conn()= default;
    void close_write() {
        try {
            (*this)->shutdown((*this)-> shutdown_send);
//            anthems::Debug(POS,TIME,__func__);
        }catch (const std::exception&e){
            anthems::Debug(POS,TIME,e.what());
        }
    }
    void close_both(){
        try {
            (*this)->shutdown((*this)-> shutdown_both);
//            anthems::Debug(POS,TIME,__func__);
        }catch (const std::exception&e){
            anthems::Debug(POS,TIME,e.what());
        }
    }
    void close_read() {
        try {
            (*this)->shutdown((*this)-> shutdown_receive);
//            anthems::Debug(POS,TIME,__func__);
        }catch (const std::exception&e){
            anthems::Debug(POS,TIME,e.what());
        }
    }
    void close(){
        try {
            (*this)->close();
            anthems::Debug(POS,TIME,__func__);
        }catch (const std::exception&e){
            anthems::Debug(POS,TIME,e.what());
        }
    }

    anthems::bytes read_all(std::size_t n);
    std::size_t write_all(anthems::bytes&data);

    virtual std::size_t read( anthems::bytes& buf);
    virtual std::size_t write( anthems::bytes &buf);

};

size_t pipe_then_close( anthems::ss_conn src, anthems::ss_conn dst, const std::string &debug_name);

}
#endif // !ANTHEMS_SSCONN_HPP
