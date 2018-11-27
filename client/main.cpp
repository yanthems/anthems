
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

void handle(anthems::ss_conn &&conn,const anthems::tcp_client& client) {
    anthems::Debug(POS, TIME, __func__);
    auto rc_client = const_cast<anthems::tcp_client &>(client);
    auto host = std::string("127.0.0.1");
//    host=std::string("ss2.fuckneusoft.com");
    auto port = std::string("23334");
    auto cipher = anthems::cipher("aes-256-cfb", "test");
    try {
        //复制加密方式
        auto mcip = cipher;
        //转为sockv5协议
        anthems::sockv5 s5 = anthems::sockv5(std::forward<anthems::ss_conn>(conn));
        try {
            //初始化链接
            s5.init();
        } catch (const std::exception &e) {
            anthems::Debug(POS, TIME, e.what());
            //初始化失败
            return;
        }
        //链接服务器
        auto c = std::move(rc_client.connect(host, port));
        //创建加密链接
        auto cip_c = anthems::cipher_conn(std::move(c), std::move(mcip));
        //写入请求
        try {
            auto req = s5.get_request();
            cip_c.write(req);
        } catch (const std::exception &e) {
            anthems::Debug(POS, TIME, e.what());
            return;
        }

        std::future<std::size_t>f1,f2;
        f1 = std::async(anthems::pipe_then_close,s5, cip_c, "local say");
        f2 = std::async(anthems::pipe_then_close,cip_c, s5, "server say");
        try {
            anthems::Debug("local count=", f1.get());
        }catch (const std::exception &e) {
            anthems::Warning(POS,TIME, e.what());
        }
        try {
            anthems::Debug("server count=", f2.get());
        }catch (const std::exception &e) {
            anthems::Warning(POS,TIME, e.what());
        }
        anthems::Debug("====try close cipher conn=====");

    } catch (const std::exception &e) {
        anthems::Debug(POS, TIME, e.what());
    }
}
void listen(const std::string&port) {
    anthems::tcp_server server(port, anthems::tcpv4);
    anthems::Debug(POS,TIME,"listen port:",port);
    anthems::tcp_client client;
//    thread_pool tp(4);
//    safe_queue<std::future<void>> ts;
    std::vector<std::future<void>>ts;
//    std::thread t1([&](){
//        while(true){
//            ts.pop_front().get();
//        }
//    });
    try {
        while (true) {
            auto conn = server.accept();
            ts.emplace_back(std::async(handle,std::move(conn),client));
        }
    } catch (const std::exception &e) {
        anthems::Debug(POS,TIME,e.what());
    }
//    t1.join();
}

int main() {

    listen("9999");
}

