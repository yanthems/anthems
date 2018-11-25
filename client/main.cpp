
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

void handle(anthems::ss_conn &&conn, const std::string &host, const std::string &port) {
    try {
        anthems::tcp_client client;
        auto cipher = anthems::cipher("aes-256-cfb", "test23334");
        auto s5 = anthems::sockv5(std::forward<anthems::ss_conn>(conn));

        //复制加密方式
        auto mcip = cipher;
        anthems::ss_conn c;
        try {
            //链接服务器
            c = client.connect(host, port);
        } catch (const std::exception &e) {
            anthems::log(e.what());
            //失败关闭sockv5
            s5.close_write();
            return;
        }
        //创建加密链接
        auto cip_c = anthems::cipher_conn(std::move(c), std::move(mcip));
        //写入请求
        auto req=s5.get_request();
        cip_c.write(req);

        auto f1 = std::async([&]() {
            return anthems::pipe_then_close(s5, cip_c, "local say:");
        });
        anthems::pipe_then_close(cip_c, s5, "remote say:");

        anthems::log("local count=", f1.get());
        anthems::log("====try close cipher conn=====");
    } catch (const std::exception &e) {
        anthems::log(e.what());
    }
}
void listen(const std::string&port) {

    anthems::tcp_server server(port, anthems::tcpv4);
    thread_pool tp(4);
    std::vector<std::future<void>> ts;
    try {
        while (true) {
            auto conn = server.accept();
//                handle(std::move(conn),"127.0.0.1","23334");
            ts.emplace_back(std::async(handle,std::move(conn), "127.0.0.1", "23334"));
        }
    } catch (const std::exception &e) {
        anthems::log(e.what());
    }

}

int main() {
    listen("1088");
}

