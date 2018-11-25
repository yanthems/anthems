

#include "cipher.hpp"
#include "ss_conn.hpp"

#include "logger.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"
#include "safe_queue.hpp"
#include "protocol.hpp"

#include <future>
#include <ssc.hpp>
#include <helper.hpp>
#include <thread_pool.hpp>
#include <thread>

void handle(anthems::cipher_conn&& cip_c) {
    auto client = anthems::tcp_client();
    try {
        //读取加密请求
        auto[host, port]=cip_c.parse_addr();
        anthems::ss_conn remote;
        //尝试连接请求服务器
        try {
            remote = std::move(client.connect(host, port));
        } catch (const std::exception &e) {
            anthems::log(e.what());
            //失败关闭加密连接
            cip_c.close_read();
            return;
        }
        auto f1 = std::async([&]() {
            return anthems::pipe_then_close(cip_c, remote, "local say");
        });
        anthems::pipe_then_close(remote, cip_c, "server say");
        anthems::log("local count=", f1.get());

        anthems::log("====try close cipher conn=====");
    } catch (const std::exception &e) {
        anthems::log(e.what());
    }
}
void proxy(const std::string&port) {
    auto server = anthems::tcp_server(port, anthems::tcpv4);
    auto cipher = anthems::cipher("aes-256-cfb", "test23334");
    std::vector<std::future<void>>ts;
    try {
        while (true) {
            auto cip = cipher;
            auto cip_c = anthems::cipher_conn(server.accept(), std::move(cip));
            ts.emplace_back(std::async(handle,std::move(cip_c)));
        }

    } catch (const std::exception &e) {
        anthems::log(e.what());
    }

}
int main() {
    proxy("23334");
}