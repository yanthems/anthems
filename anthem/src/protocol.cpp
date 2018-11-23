
#include "protocol.hpp"
#include "logger.hpp"
namespace anthems{

sockv5::sockv5(anthems::ss_conn &&c)
:conn(std::forward<ss_conn>(c)) {
    hand_shake();
    do_request();
    do_response();
}
sockv5::~sockv5() {
    conn->close();
}
void sockv5::hand_shake(){
    /* request
-------------------------------------
|	VER		NMETHODS	METHODS		|
|	1byte	1byte		1byte		|
-------------------------------------
*/
    log(__func__);
    auto res = conn.read_enough(3);
    if (res[0] != 0x05) {
        anthems::log("not socksV5");
        return;
    } else {
        switch (res[2]) {
            //no password
            case 0x00: {
                conn.write(anthems::bytes({0x05, 0x00}));
            }
                break;
                //security interface
            case 0x01: {
                conn.write(anthems::bytes({0x05, 0x01}));
            }
                break;
                //username + password
            case 0x03: {
                conn.write(anthems::bytes({0x05, 0x02}));
            }
                break;
                // 0x03 ~ 0x7F IANA allocation
                // 0x80 ~ 0xFE private method
            case 0xff: {
                anthems::log("no method");
                return;
            }
        }
    }
//        conn.write(anthems::bytes({0x05, 0x00}));
}

void sockv5::do_request(){
    /* request
-------------------------------------------------------------
|	VER		CMD		RSV		ATYP	DST.ADDR	DST.PORT	|
|	1byte	1byte	1byte	1byte	n byte		2byte		|
-------------------------------------------------------------
*/
// 1 + 1 + 1 + 1 + (1+255) + 2
    log(__func__);
    auto res = conn.read(3);
    if(res[0]!=0x05){
        anthems::log("not socksV5");
    }
    if(res[1]!=0x01){
        anthems::log("socks command not supported");
    }
    do_parse();
}
void sockv5::do_response(){
    log(__func__);
    auto rsp=anthems::bytes(10);
    //socket version
    rsp[0]=0x05;
    //response status
    //	0x00	成功
    //	0x01	普通的失败
    //	0x02	规则不允许的连接
    //	0x03	网络不可达
    //	0x04	主机不可达
    //	0x05	连接被拒绝
    //	0x06	TTL超时
    //	0x07	不支持的命令
    //	0x08	不支持的地址类型
    //	0x09 ~ 0xFF		未定义
    rsp[1]=0x00;
    //RSV must 0x00
    rsp[2]=0x00;
    //address type
    //	0x01	IPV4
    //	0x03	域名
    //	0x04	IPV6
    rsp[3]=0x01;
    // IPV4为4字节 域名为字符串 IPV6为16字节
    rsp[4]= rsp[5]= rsp[6]=rsp[7] = 0x00;
    //bind port
    rsp[8]=0x00,rsp[9]=0x00;
    conn.write(rsp);
}
static const constexpr auto typeIPv4 = 0x01; // type is ipv4 address
static const constexpr auto typeDM   = 0x03; // type is domain address
static const constexpr auto typeIPv6 = 0x04; // type is ipv6 address
void sockv5::do_parse() {
    rawreq=do_parse(conn);
    anthems::log("req len=",rawreq.size());
}
bytes sockv5::do_parse(anthems::ss_conn &c) {
    const constexpr auto  IPv4len = 4;
    auto DMlen=0;//dynamic
    const constexpr auto  IPv6len = 16;
    const constexpr auto PortLen=2;
    auto len=0;
    auto typeAddr=c.read(1);
    bytes dm;
    switch (typeAddr[0]){
        case typeIPv4:
            len+=IPv4len;
            break;
        case typeIPv6:
            len+=IPv6len;
        case typeDM:{
            dm=c.read(1);
            DMlen=dm[0];
            len+=DMlen;
        }
    }
    len+=PortLen;
    return typeAddr+dm+c.read(len);
}
std::tuple<std::string,std::string> sockv5::parse_addr(anthems::bytes& req){

    const constexpr auto  IPv4len = 4;
    auto DMlen=0;//dynamic
    const constexpr auto  IPv6len = 16;

    const constexpr auto IPstart=1;
    const constexpr auto DMstart=IPstart+1;

    auto finish=0;//dynamic

    std::string host,port;

    switch (req[0]){
        case typeIPv4:{
            finish=IPstart+IPv4len;
            anthems::bytes ipv4=req.split(IPstart,finish);
            asio::ip::address_v4 p4(ipv4.to_array<4>());
            host=p4.to_string();
            port=std::to_string(req[finish]<<8|req[finish+1]);
        }
            break;
        case typeDM:{
            DMlen=req[IPstart];
            finish=DMstart+DMlen;
            anthems::bytes dm=req.split(DMstart,finish);
            host=dm.to_string();
            port=std::to_string(req[finish]<<8|req[finish+1]);

        }
            break;
        case typeIPv6:{
            finish=IPstart+IPv6len;
            anthems::bytes ipv6=req.split(IPstart,finish);
            asio::ip::address_v6 p6(ipv6.to_array<16>());
            host=p6.to_string();
            port=std::to_string(req[finish]<<8|req[finish+1]);
        }
            break;
    }
    anthems::log("req len=",req.size(),"host=>",host,"<=","port=>",port,"<=");
    return std::make_tuple(host,port);
}

simple::simple(anthems::ss_conn &&c)
:conn(std::forward<ss_conn>(c)){

}
simple::~simple() {
    conn->close();
}

}//anthems
