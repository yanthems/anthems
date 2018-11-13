
#include "ssc.hpp"
#include "logger.hpp"
namespace anthems{

    ssclient::ssclient(const std::string &host, const std::string &port) {

        m_serv=new asio::io_service;
        m_sock=new asio::ip::tcp::socket(*m_serv);
        asio::ip::tcp::resolver resolver(*m_serv);
        asio::ip::tcp::resolver::query query(host,port);
        auto ran=resolver.resolve(query);
        if(ran.size()==0){
            anthems::log("err");
        }
        for(const auto&it:ran){
            anthems::log(it.host_name());
//            m_sock.connect(it);
        }
        //connect the first
        m_sock->connect(*ran.begin());
    }

    ssclient::~ssclient(){
        delete m_sock;
        delete m_serv;
    }

    std::size_t  ssclient::write(const std::string &msg) {
        asio::const_buffer buf(msg.data(),msg.size());
//        asio::const_buffers_1
        return m_sock->send(buf);
    }
    std::size_t ssclient::write(const char *ptr) {
//        m_sock->send(ptr);
        asio::const_buffer buf(ptr, std::strlen(ptr));
        return m_sock->send(buf);
    }
    std::size_t ssclient::write(const anthems::bytes& data) {
        asio::const_buffer buf(data.data(),data.size());
        return m_sock->send(buf);
    }

    anthems::bytes ssclient::read() {
        auto res=anthems::bytes(2048);
        auto buf=asio::buffer(res);
        auto n=m_sock->receive(buf);
        anthems::log("n =",n);
        return res;
    }

}