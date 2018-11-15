#include "ss_conn.hpp"

#include "asio.hpp"

#include "logger.hpp"

namespace anthems {

ss_conn::ss_conn(asio::io_service &io)
        : super(std::make_shared<asio_socket_raw>(io))
{
}

anthems::bytes ss_conn::read(std::size_t n) {
    auto res = anthems::bytes(n);
    auto buf = asio::buffer(res);
    (*this)->read_some(buf);
    return res;
}

anthems::bytes ss_conn::read_all() {
    std::size_t len = anthems::bytes::Block_Size;
    anthems::bytes res;
    asio::error_code err;
    while (true) {
        auto tmp = anthems::bytes(len);
        auto buf = asio::buffer(tmp);
        auto n = (*this)->read_some(buf, err);
        res += tmp;
        if (n < len) {
            break;
        }
        if (err.value() == 0) {
            continue;
        }
        if (err.value() == asio::error::eof) {
            break;
        } else {
            asio::detail::throw_error(err, "receive");
        }
    }
    return res;
}

std::size_t ss_conn::write(const anthems::bytes &data) {
    asio::const_buffer buf(data.data(), data.size());
    return (*this)->write_some(buf);
}

#if 0
std::size_t ss_conn::write(const char *data) {
    asio::const_buffer buf(data, std::strlen(data));
    return (*this)->write_some(buf);
}

std::size_t ss_conn::write(const std::string &data) {
    asio::const_buffer buf(data.data(), data.size());
    return (*this)->write_some(buf);
}
#endif

size_t ss_conn::pip_then_close(anthems::ss_conn &src,anthems::ss_conn &dst) {

    size_t res=0;
    try {
        while (true){
            auto res=src.read(ss_conn::Block);
            res+=dst.write(res);
        }
    }catch (const std::exception&e){
        anthems::log(e.what());
    }

    return res;
}

}