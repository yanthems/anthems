#include "ss_conn.hpp"

#include "asio.hpp"

#include "logger.hpp"

#include <memory>
namespace anthems {

ss_conn::ss_conn(asio::io_service &io)
//        : super(std::make_shared<asio_socket_raw>(io)) {

:super (std::shared_ptr<asio_socket_raw>(new asio_socket_raw(io),[](asio_socket_raw*ptr){
    anthems::Debug(TIME,"close socket ?");
    try {
        ptr->shutdown(ptr->shutdown_both);
    }catch (const std::exception&e){
        anthems::Warning(POS,TIME,e.what());
    }
    try {
        ptr->close();
    }catch (const std::exception&e){
        anthems::Warning(POS,TIME,e.what());
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
std::size_t ss_conn::write_all(anthems::bytes &data) {
    std::size_t l=0,r=data.size();
    while (r){
        auto buf=asio::buffer(data.data()+l,r);
        auto len=(*this)->send(buf);
        l+=len;
        r-=len;
    }
    return l;
}

std::size_t ss_conn::read( anthems::bytes& data) {
    auto buf =asio::buffer(data.data(),data.size());
    return (*this)->receive(buf);
}

std::size_t ss_conn::write( anthems::bytes &data) {
    return write_all(data);

    auto buf =asio::buffer(data.data(),data.size());
    return (*this)->send(buf);
}

//@test http://pewpewpew.work/images/logo.gifs
size_t pipe_then_close(anthems::ss_conn src, anthems::ss_conn dst, const std::string &debug_name) {
//    anthems::Debug(POS,TIME,debug_name);
    size_t len = 0;
    bool readerr=false;

    while (!readerr) {
        anthems::bytes buf(ss_conn::Block);
        size_t l=0;
        try {
//            anthems::Debug(TIME,debug_name, "======start read======>",l);
             l=src.read(buf);
//            anthems::Debug(TIME,debug_name, "read======>>>",l);
        } catch (const std::exception &e) {
//            anthems::Debug(POS,TIME,debug_name,e.what());
            readerr=true;
        }
        try {
//            anthems::Debug("==>",buf,"<==");
//            anthems::Debug(TIME,debug_name, "======start write======>",l);
            if(l) {
                buf.resize(l);
                l = dst.write(buf);
                len +=l;
            }
//            anthems::Debug(TIME,debug_name, l,"<<<======write");

        } catch (const std::exception &e) {
//            anthems::Debug(POS,TIME,debug_name,e.what());
            break;
        }
//        anthems::Debug(TIME,debug_name, "<============total=========>",len);
    }
    anthems::Debug(POS,TIME,debug_name, "[total]",len);
    return len;
}

}