
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

void handle(anthems::ss_conn &&conn,anthems::tcp_client& client) {
    anthems::Debug(POS,TIME,__func__);

    auto host=std::string("127.0.0.1");
    auto port=std::string("23334");
    auto cipher = anthems::cipher("aes-256-cfb", "test23334");
    [&]() {
        try {
            //复制加密方式
            auto mcip = cipher;
            //转为sockv5协议
            anthems::sockv5 s5=anthems::sockv5(std::forward<anthems::ss_conn>(conn));
            try {
                //初始化链接
                s5.init();
            } catch (const std::exception &e) {
                anthems::Debug(POS, TIME, e.what());
                //初始化失败
                s5.close_write();
                return;
            }
                //链接服务器
                auto c = std::move(client.connect(host, port));
                //创建加密链接
                auto cip_c = anthems::cipher_conn(std::move(c), std::move(mcip));
                //写入请求
                auto req = s5.get_request();
                cip_c.write(req);

                auto f1 = std::async([&]() {
                    return anthems::pipe_then_close(s5, cip_c, "local say:");
                });
                anthems::pipe_then_close(cip_c, s5, "remote say:");

                anthems::Debug("local count=", f1.get());
                anthems::Debug("====try close cipher conn=====");
        } catch (const std::exception &e) {
            anthems::Debug(POS, TIME, e.what());
        }
    }();
}
void listen(const std::string&port) {
    anthems::tcp_server server(port, anthems::tcpv4);
    anthems::Debug(POS,TIME,"listen port:",port);
    anthems::tcp_client client;
    thread_pool tp(4);
    safe_queue<std::future<void>> ts;
    std::thread t1([&](){
        while(true){
            ts.pop_front().get();
        }
    });
    try {
        while (true) {
            auto conn = server.accept();
//                handle(std::move(conn),client);
            ts.push(std::async(handle,std::move(conn),client));
        }
    } catch (const std::exception &e) {
        anthems::Debug(POS,TIME,e.what());
    }
    t1.join();
}

int main() {

    listen("9999");
}

