#ifndef ANTHEMS_SSC_HPP
#define ANTHEMS_SSC_HPP

#include "asio.hpp"
#include "bytes.hpp"

namespace anthems{

    class ssclient{
    public:

        ssclient(const std::string& host,const std::string&port);
        virtual ~ssclient();

        std::size_t write(const std::string& msg);
        std::size_t write(const anthems::bytes& data);
        std::size_t write(const char* ptr);

        anthems::bytes read();

    private:
        asio::io_service *m_serv;
        asio::ip::tcp::socket *m_sock;

    };

}

#endif //ANTHEMS_SSC_HPP
