#include "ssconn.hpp"

#include "asio.hpp"

namespace anthems {


void ssconn::test() {

    asio::io_context io;
    while (true) {
        try {
            io.run();
        } catch (std::exception& e) {
            anthems::log(e.what());
        }
    }
}

}