
#ifndef ANTHEMS_PROTOCOL_HPP
#define ANTHEMS_PROTOCOL_HPP

#include "ss_conn.hpp"
#include "bytes.hpp"

namespace anthems{

class sockv5:public ss_conn{
    using super=ss_conn;
public:
    sockv5(ss_conn&&c);
    bytes get_request(){
        return rawreq;
    }
public:
    static std::tuple<std::string,std::string>parse_addr(bytes& req);
    static bytes do_parse(ss_conn&c);
public:
    //1 type | 1 len | 255 max | 2 port 
    static const unsigned int ReqMax=259;
    //1 type | 1 len | 1 min | 2 port
    static const unsigned int ReqMin=5;
    void init();
private:
    void hand_shake();
    void do_parse();
    void do_request();
    void do_response();

private:
    bytes rawreq;
};


}

#endif //ANTHEMS_PROTOCOL_HPP
