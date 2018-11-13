
#ifndef ANTHEMS_SSS_HPP
#define ANTHEMS_SSS_HPP

#include <asio.hpp>
#include <string>
namespace anthems {

    class ssserver {
    public:

        ssserver(const std::string&port);
        virtual ~ssserver();

        void run();

    private:
        asio::io_service *m_serv;
        unsigned int m_port;
    };
}

#endif //ANTHEMS_SSS_HPP
