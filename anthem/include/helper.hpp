
#ifndef ANTHEMS_HELPER_HPP
#define ANTHEMS_HELPER_HPP

#include "bytes.hpp"
#include "asio.hpp"
#include "logger.hpp"
namespace anthems{


std::tuple<std::string,std::string> parse_addr(anthems::bytes res){
    const constexpr auto typeIPv4 = 0x01; // type is ipv4 address
    const constexpr auto typeDM   = 0x03; // type is domain address
    const constexpr auto typeIPv6 = 0x04; // type is ipv6 address

    const constexpr auto  IPv4len = 4;
    auto DMlen=0;//dynamic
    const constexpr auto  IPv6len = 16;

    const constexpr auto IPstart=1;
    const constexpr auto DMstart=IPstart+1;

    auto finish=0;//dynamic

    std::string host,port;

    switch (res[0]){
        case typeIPv4:{
            finish=IPstart+IPv4len;
            anthems::bytes ipv4=res.split(IPstart,finish);
            asio::ip::address_v4 p4(ipv4.to_array<4>());
            port=std::to_string(res[finish]<<8|res[finish+1]);
            host=p4.to_string();
        }
            break;
        case typeDM:{
            DMlen=res[IPstart];
            finish=DMstart+DMlen;
            anthems::bytes dm=res.split(DMstart,finish);
            host=dm.to_string();
            port=std::to_string(res[finish]<<8|res[finish+1]);

        }
            break;
        case typeIPv6:{
            finish=IPstart+IPv6len;
            anthems::bytes ipv6=res.split(IPstart,finish);
            asio::ip::address_v6 p6(ipv6.to_array<16>());
            host=p6.to_string();
            port=std::to_string(res[finish]<<8|res[finish+1]);
        }
            break;
    }
    anthems::log("host=>",host,"port=>",port);
    return std::make_tuple(host,port);
}

}

#endif //ANTHEMS_HELPER_HPP
