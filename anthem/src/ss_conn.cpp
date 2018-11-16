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
    auto len=(*this)->read_some(buf);
    if(len<n){
        anthems::log("len=",len,"n=",n);
        res.resize(len);
    }
    return res;
}

anthems::bytes ss_conn::read_enough(std::size_t n) {
    auto res = anthems::bytes(n);
    auto buf = asio::buffer(res);
    auto len=(*this)->read_some(buf);
    return res;
}

std::tuple<anthems::bytes,std::size_t> ss_conn::read_length(std::size_t n) {
    auto res = anthems::bytes(n);
    auto buf = asio::buffer(res);
    auto len=(*this)->read_some(buf);
    return std::make_tuple(res,len);
}


std::size_t ss_conn::write(const anthems::bytes &data) {
    asio::const_buffer buf(data.data(), data.size());
    return (*this)->write_some(buf);
}

#if 0
anthems::bytes ss_conn::read(std::size_t n, asio::error_code &err) {
    auto res = anthems::bytes(n);
    auto buf = asio::buffer(res);
    (*this)->read_some(buf,err);
    return res;
}

std::size_t ss_conn::write(const anthems::bytes &data, asio::error_code &err) {
    asio::const_buffer buf(data.data(), data.size());
    return (*this)->write_some(buf,err);
}
#endif


size_t ss_conn::pipe_then_close(anthems::ss_conn &src, anthems::ss_conn &dst, const std::string &debug_name) {

    size_t len=0;
    anthems::bytes res;
        while (true){
            try {
                res=std::move(src.read(ss_conn::Block));
            }catch (const std::exception&e){
                anthems::log(e.what(),debug_name);
                break;
            }
            anthems::log(debug_name,"->",res);
            try {
                len+=dst.write(res);
            }catch (const std::exception&e){
                anthems::log(e.what(),debug_name);
                break;
            }
        }
    return len;
}

#if 0
std::size_t ss_conn::bib_then_close(anthems::ss_conn &src, anthems::ss_conn &dst) {
    size_t len=0;
    anthems::bytes res;
    asio::error_code err;
    while (true){
        res=std::move(src.read(ss_conn::Block,err));
        if(err.value()==asio::error::eof){
            break;
        } else if(err.value()!=0){
            anthems::log(err.message());
            break;
        }
        len+=dst.write(res,err);
        if(err.value()==asio::error::eof){
            break;
        }else if(err.value()!=0){
            anthems::log(err.message());
            break;
        }
    }
}
#endif

}