
#include "logger.hpp"
#include "ssc.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

#include "cipher.hpp"
#include "protocol.hpp"

#include "protocol.hpp"
#include "safe_queue.hpp"


#include "ThreadPool.h"
#include <thread_pool.hpp>
#include <thread>
#include <future>
#include <chrono>


//un support
//https://www.gstatic.com/og/_/js/k=og.og2.en_US.ZMe_WxpNhwk.O/rt=j/m=drt,def/exm=in,fot/d=1/ed=1/rs=AA2YrTujvSasDy0loAQWKMWHJLx-fNDIuA
//
void handle(anthems::ss_conn &&conn,const anthems::tcp_client& c_client) {
    anthems::Debug(POS, TIME, __func__);
    auto client = const_cast<anthems::tcp_client &>(c_client);
    auto host = std::string("127.0.0.1");
//    host = std::string("ss3.fuckneusoft.com");
    auto port = std::string("12345");
    auto cipher = anthems::cipher("aes-256-cfb", "test");
    bool isClosed = false;
//复制加密方式
    auto mcip = cipher;

    anthems::defer d1{[&]() {
        if (!isClosed) {
            anthems::log(POS, "[close conn]");
            conn.close_both();
            conn.close();
        }
    }};
    anthems::ss_conn other;
    try {
        other = client.connect(host, port);
    }catch (const std::exception&e){
        anthems::Debug(POS,TIME,e.what());
        return;
    }

    try {
        //创建加密链接
        auto cip_c = anthems::cipher_conn(std::move(other), std::move(mcip));
        anthems::defer d2{[&]() {
            if (!isClosed) {
                anthems::Debug(POS, "[close cip_c]");
                cip_c.close_both();
                cip_c.close();
            }
        }};
        auto req=anthems::sockv5::hand_shake(conn);
        //初始化链接
        cip_c.write(req);
        std::future<std::size_t> f1, f2;
        f1 = std::async(anthems::pipe_then_close, conn, cip_c, "local say");
        f2 = std::async(anthems::pipe_then_close, cip_c, conn, "server say");
        anthems::Debug(POS, TIME, "local count=", f1.get());
        anthems::Debug(POS, TIME, "server count=", f2.get());
        anthems::Debug(POS, TIME, "====try close cipher conn=====");
        isClosed = true;
    } catch (const std::exception &e) {
        anthems::Debug(POS, TIME, e.what());
    }
}
void listen(const std::string&lport) {
    anthems::tcp_server server(lport, anthems::tcpv4);
    anthems::Debug(POS, TIME, "listen port:", lport);
    anthems::tcp_client client;
    thread_pool tp(4);

    ThreadPool TP(4);
    
    while (true) {
//        handle(server.accept(),client);
//        tp.add(handle, std::move(server.accept()), client);
        TP.add(handle, std::move(server.accept()), client);
    }

}




int main() {


    listen("9999");
}

