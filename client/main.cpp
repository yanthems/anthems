
#include "logger.hpp"
#include "ssc.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

#include "cipher.hpp"

#include <thread_pool.hpp>
#include <future>
#include <helper.hpp>

int handle(anthems::ss_conn&& conn) {

    anthems::tcp_client client;
    auto cipher=anthems::cipher("aes-256-cfb","test23334");

    std::string rhost="127.0.0.1";
    std::string rport="23334";
    std::string host;
    std::string port;

    auto hand_shake = [&]() {
/* request
-------------------------------------
|	VER		NMETHODS	METHODS		|
|	1byte	1byte		1byte		|
-------------------------------------
*/
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
    };

    auto get_request=[&](){
/* request
-------------------------------------------------------------
|	VER		CMD		RSV		ATYP	DST.ADDR	DST.PORT	|
|	1byte	1byte	1byte	1byte	n byte		2byte		|
-------------------------------------------------------------
*/
// 1 + 1 + 1 + 1 + (1+255) + 2
        auto res = conn.read_enough(263);
        auto rawreq=anthems::bytes{};
        if(res[0]!=0x05){
            anthems::log("not socksV5");
            return rawreq;
        }
        if(res[1]!=0x01){
            anthems::log("socks command not supported");
            return rawreq;
        }
        const constexpr auto AddressIndex=3;
        rawreq=res.split(AddressIndex);
         auto [thost,tport]=anthems::parse_addr(rawreq);
        host=thost;
        port=tport;
        return rawreq;
    };

    auto do_response=[&](){
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
        rsp[8]=0x08,rsp[9]=0x43;
        conn.write(rsp);
    };

    auto create_cipconn=[&](anthems::bytes&&helloData){
        if(host.empty()||port.empty()){
            throw std::logic_error("host or port is empty");
        }
        auto mcip=cipher;
        auto cip_c=anthems::cipher_conn(client.connect(rhost,rport),std::move(mcip));

        cip_c.write(helloData);
        return cip_c;
    };

    try {
        anthems::log("start hand shake");
        hand_shake();
        anthems::log("get request");
        auto helloData=get_request();
        do_response();
        auto cip_c=create_cipconn(std::move(helloData));

        auto f1=std::async([&](){
            return anthems::ss_conn::pipe_then_close(conn,cip_c,"local say:");
        });
//        auto f2=std::async([&](){
//            return anthems::ss_conn::pipe_then_close(cip_c,conn,"remote say:");
//        });
//        anthems::log("local count=",f1.get(),"sever count=",f2.get());

        anthems::ss_conn::pipe_then_close(cip_c,conn,"remote say:");
        f1.get();
        conn->shutdown(conn->shutdown_both);
        cip_c->shutdown(cip_c->shutdown_both);
        anthems::log("==try close==");
        conn->close();
        cip_c->close();
    }catch (const std::exception&e){
        anthems::log(e.what());
    }

}
void listen(const std::string&port){

    anthems::tcp_server server(port,anthems::tcpv4);
    thread_pool tp(4);

    std::vector<std::future<int>>res;

    while (true){
        res.emplace_back(tp.add(handle,std::move(server.accept())));
    }

    for(auto &&i:res){
        i.get();
    }
}

int main(){

    listen("1088");
}