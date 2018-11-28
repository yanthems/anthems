
#include "logger.hpp"
#include "ssc.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

#include "cipher.hpp"
#include "protocol.hpp"

#include "protocol.hpp"
#include "safe_queue.hpp"


#include <thread_pool.hpp>
#include <thread>
#include <future>
#include <chrono>


//un support
//https://www.gstatic.com/og/_/js/k=og.og2.en_US.ZMe_WxpNhwk.O/rt=j/m=drt,def/exm=in,fot/d=1/ed=1/rs=AA2YrTujvSasDy0loAQWKMWHJLx-fNDIuA
//
void handle(anthems::ss_conn conn,const anthems::tcp_client& c_client) {
    anthems::Debug(POS, TIME, __func__);
    auto client = const_cast<anthems::tcp_client &>(c_client);
    auto host = std::string("127.0.0.1");
//    host = std::string("ss3.fuckneusoft.com");
    auto port = std::string("12345");
    auto cipher = anthems::cipher("aes-256-cfb", "test");
//复制加密方式
    auto mcip = cipher;

    auto other = client.connect(host, port);
    try {
        //创建加密链接
        auto cip_c = anthems::cipher_conn(other, std::move(mcip));
        auto req = anthems::sockv5::hand_shake(conn);
        //初始化链接
        cip_c.write(req);
        std::future<std::size_t> f1, f2;
        f1 = std::async(anthems::pipe_then_close, conn, cip_c, "local say");
        f2 = std::async(anthems::pipe_then_close, cip_c, conn, "server say");
        anthems::Debug(POS, TIME, "local count=", f1.get());
        anthems::Debug(POS, TIME, "server count=", f2.get());
        anthems::Debug(POS, TIME, "====try close cipher conn=====");
    } catch (const std::exception &e) {
        anthems::Debug(POS, TIME, e.what());
    }
}
void listen(const std::string&lport) {
    anthems::tcp_server server(lport, anthems::tcpv4);
    anthems::Debug(POS, TIME, "listen port:", lport);
    anthems::tcp_client client;
    thread_pool tp(100);


    while (true) {
        anthems::ss_conn conn;
        try {
            conn = server.accept();
        } catch (const std::exception &e) {
            anthems::Debug(POS, TIME, e.what());
            continue;
        }
        tp.add(handle, conn, client);
    }

}




int main() {

    listen("9999");
}

