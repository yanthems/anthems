
#ifndef ANTHEMS_PROTOCOL_HPP
#define ANTHEMS_PROTOCOL_HPP

#include "ss_conn.hpp"
#include "bytes.hpp"

namespace anthems{

class sockv5{
public:
    sockv5(ss_conn&&c);
    ~sockv5();
    bytes get_request(){
        return rawreq;
    }
    ss_conn& get(){
        return conn;
    }
public:
    static std::tuple<std::string,std::string>parse_addr(bytes& req);
    static bytes do_parse(ss_conn&c);
public:
    //1 type | 1 len | 255 max | 2 port 
    static const unsigned int RequestSize=259;
private:
    void hand_shake();
    void do_parse();
    void do_request();
    void do_response();

private:
    ss_conn conn;
    bytes rawreq;
};

class simple{
public:
    simple(ss_conn&&c);
    ~simple();
    ss_conn& get(){
        return conn;
    }
private:
    ss_conn conn;
};



}

#endif //ANTHEMS_PROTOCOL_HPP
