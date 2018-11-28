

#include "cipher.hpp"
#include "ss_conn.hpp"

#include "logger.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"
#include "safe_queue.hpp"
#include "protocol.hpp"
#include "helper.hpp"

#include <future>
#include <ssc.hpp>
#include <helper.hpp>
#include <thread_pool.hpp>
#include <thread>

void handle(anthems::cipher_conn cip_c,const anthems::tcp_client&const_client) {
    auto client = const_cast<anthems::tcp_client &>(const_client);
    try {
        //读取加密请求
        auto[host, port]=cip_c.parse_addr();
        //尝试连接请求服务器
        auto remote = client.connect(host, port);
        std::future<std::size_t> f1, f2;
        f1 = std::async(anthems::pipe_then_close, cip_c, remote, "local say");
        f2 = std::async(anthems::pipe_then_close, remote, cip_c, "server say");
        anthems::Debug(POS, TIME, "local count=", f1.get());
        anthems::Debug(POS, TIME, "server count=", f2.get());
        anthems::Debug(POS, TIME, "====try close cipher conn=====");
    } catch (const std::exception &e) {
        anthems::Debug(POS, TIME, e.what());
    }
}
void proxy(const std::string&port,const std::string&method,const std::string&pass) {
    anthems::Debug(POS, TIME, __func__);
    auto server = anthems::tcp_server(port, anthems::tcpv4);
    auto client = anthems::tcp_client();
    auto cipher = anthems::cipher(method, pass);

    thread_pool tp(4);
    while (true) {
        auto cip = cipher;
        anthems::ss_conn conn;
        try {
            conn = server.accept();
        } catch (const std::exception &e) {
            anthems::Debug(POS, TIME, e.what());
            continue;
        }
        auto cip_c = anthems::cipher_conn(conn, std::move(cip));
        tp.add(handle, cip_c, client);
    }
}
int main(int argc,char*argv[]) {
    auto parse = [&](const char *arg) -> std::string {
        for (auto i = 1; i < argc; i++) {
            if (std::strcmp(argv[i], arg) == 0) {
                if (i + 1 < argc) {
                    return std::string(argv[i + 1]);
                }
            }
        }
        return std::string{};
    };
    auto parse_port = [&]() {
        auto port = parse("-p");
        if (port.empty()) {
            port = std::string{"12345"};
        }
        return port;
    };
    auto parse_method = [&]() {
        auto method = parse("-m");
        if (method.empty()) {
            method = std::string{"aes-256-cfb"};
        }
        return method;
    };

    auto parse_password = [&]() {
        auto pass = parse("-k");
        if (pass.empty()) {
            pass = std::string("test");
        }
        return pass;
    };


    auto port = parse_port();
    auto method = parse_method();
    auto pass = parse_password();
    proxy(port, method, pass);
    return 0;
}