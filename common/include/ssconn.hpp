#ifndef ANTHEMS_SSCONN_HPP
#define ANTHEMS_SSCONN_HPP

#include <string>

#include "cipher.hpp"
#include "asio.hpp"
#include "asio/ip/tcp.hpp"

namespace anthems {

class ssconn {
public:
    ssconn(asio::ip::tcp::socket conn,cipher&& c);
};
}
#endif // !ANTHEMS_SSCONN_HPP
