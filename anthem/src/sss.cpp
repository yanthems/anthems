
#include "sss.hpp"
#include <future>

#include "bytes.hpp"
#include "logger.hpp"
namespace anthems{

    ssserver::ssserver(const std::string &port) {

        m_serv=new asio::io_service;
        m_port=std::stoi(port);

    }

    void ssserver::run() {
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(),m_port);
        asio::ip::tcp::acceptor acceptor(*m_serv,endpoint);
        try {
            while (true){
                auto socket=asio::ip::tcp::socket(*m_serv);
                acceptor.accept(socket);

                std::async([&](){
                    //echo
                    anthems::bytes data(2048);
                    auto buf=asio::buffer(data);
                    socket.receive(buf);
                    anthems::log("server read");
                    socket.send(buf);
                });
            }
        }catch (std::exception&e){
            anthems::log(e.what());
        }
    }
    ssserver::~ssserver() {
        delete m_serv;
    }
}