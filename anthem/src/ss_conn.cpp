#include "ss_conn.hpp"

#include "asio.hpp"

#include "logger.hpp"

#include <memory>
namespace anthems {

ss_conn::ss_conn(asio::io_service &io)
//        : super(std::move(std::make_shared<asio_socket_raw>(io))) {
:super (std::shared_ptr<asio_socket_raw>(new asio_socket_raw(io),[](asio_socket_raw*ptr){
    anthems::log("close socket ?");
    ptr->close();
})){
}

anthems::bytes ss_conn::read_all(std::size_t n) {
    bytes res;
    do{
        bytes tmp(n);
        auto l=ss_conn::read(tmp);
        tmp.resize(l);
        res+=tmp;
    }while (res.size()<n);
    return res;
}
void ss_conn::write_all(anthems::bytes &data) {
    std::size_t l=0,r=data.size();
    while (r){
        auto buf=asio::buffer(data.data()+l,r);
        auto len=(*this)->write_some(buf);
        l+=len;
        r-=len;
    }
}

std::size_t ss_conn::read( anthems::bytes& data) {
    auto buf =asio::buffer(data.data(),data.size());
    return (*this)->read_some(buf);
}

std::size_t ss_conn::write( anthems::bytes &data) {
    auto buf =asio::buffer(data.data(),data.size());
    return (*this)->write_some(buf);
}

size_t pipe_then_close(anthems::ss_conn &src, anthems::ss_conn &dst, const std::string &debug_name) {

    anthems::log(__func__,debug_name);
    size_t len = 0;
    auto read_eof=false;
    while (!read_eof) {
        anthems::bytes buf(ss_conn::Block);
        try {
            anthems::log(debug_name, "======start read======>");
            auto l=src.read(buf);
            anthems::log(debug_name, "======read======>", l);
        } catch (const std::error_code &err) {
            anthems::log(debug_name,err.message());
            read_eof=true;
        }
        try {
            anthems::log(debug_name, "======start write======>");
            len += dst.write(buf);
            if(read_eof) {
                dst.close_write();
                break;
            }
        } catch (const std::error_code &err) {
            anthems::log(debug_name,err.message());
            break;
        }
        anthems::log(debug_name, "======write======>", buf.size());
    }
    return len;
}

}