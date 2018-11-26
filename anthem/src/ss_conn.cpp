#include "ss_conn.hpp"

#include "asio.hpp"

#include "logger.hpp"

#include <memory>
namespace anthems {

ss_conn::ss_conn(asio::io_service &io)
//        : super(std::move(std::make_shared<asio_socket_raw>(io))) {
:super (std::shared_ptr<asio_socket_raw>(new asio_socket_raw(io),[](asio_socket_raw*ptr){
    anthems::Debug(TIME,"close socket ?");
    try {
        ptr->close();
    }catch (const std::exception&e){
        anthems::Warning(TIME,e.what());
    }
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
    return (*this)->receive(buf);
}

std::size_t ss_conn::write( anthems::bytes &data) {
    auto buf =asio::buffer(data.data(),data.size());
    return (*this)->send(buf);
}

size_t pipe_then_close(anthems::ss_conn &src, anthems::ss_conn &dst, const std::string &debug_name) {

    anthems::Debug(TIME,debug_name);
    size_t len = 0;
    auto read_eof=false;
    while (!read_eof) {
        anthems::bytes buf(ss_conn::Block);
        try {
            anthems::Debug(TIME,debug_name, "======start read======>");
            auto l=src.read(buf);
            anthems::Debug(TIME,debug_name, "======read======>", l);
        } catch (const std::error_code &err) {
            anthems::Debug(TIME,debug_name,err.message());
            read_eof=true;
        }
        try {
            anthems::Debug(TIME,debug_name, "======start write======>");
            len += dst.write(buf);
            if(read_eof) {
                dst.close_write();
                break;
            }
        } catch (const std::error_code &err) {
            anthems::Debug(TIME,debug_name,err.message());
            break;
        }
        anthems::Debug(TIME,debug_name, "======write======>", buf.size());
    }
    return len;
}

}