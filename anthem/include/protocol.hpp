
#ifndef ANTHEMS_PROTOCOL_HPP
#define ANTHEMS_PROTOCOL_HPP

#include "ss_conn.hpp"
#include "bytes.hpp"

namespace anthems{

class sockv5{
public:
public:
    static std::tuple<std::string,std::string>parse_addr(bytes& req);
    static bytes hand_shake(anthems::ss_conn &);

public:
    //1 type | 1 len | 255 max | 2 port | 10
    static const unsigned int ReqMax=259;
private:
    static bytes do_parse(anthems::ss_conn &);
    static bytes do_request(anthems::ss_conn &);
    static void do_response(anthems::ss_conn &);

};


}

#endif //ANTHEMS_PROTOCOL_HPP
