

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

#ifdef USE_LIBGO
#include <libgo/libgo.h>
#else

#endif
void handle(anthems::cipher_conn cip_c,const anthems::tcp_client&const_client) {
    auto client = const_cast<anthems::tcp_client &>(const_client);
    try {
        //读取加密请求
        auto[host, port]=cip_c.parse_addr();
        //尝试连接请求服务器
        auto remote = client.connect(host, port);


#ifdef USE_LIBGO
        go [=](){
            anthems::pipe_then_close(cip_c,remote,"local:");
        };
        anthems::pipe_then_close(remote,cip_c,"server:");
#else
        auto f1 = std::async(anthems::pipe_then_close, cip_c, remote, "local say");
        anthems::pipe_then_close(remote, cip_c, "server say");
        anthems::Debug(POS, TIME, "local count=", f1.get());
#endif
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

#ifdef USE_LIBGO

#else
    thread_pool tp(100);
#endif
    while (true) {
        auto cip = cipher;
        try {
            auto cip_c = anthems::cipher_conn(server.accept(), std::move(cip));
#ifdef USE_LIBGO
            go [=]{
                handle(cip_c,client);
            };
#else
            tp.add(handle, cip_c, client);
#endif
        } catch (const std::exception &e) {
            anthems::Debug(POS, TIME, e.what());
            continue;
        }

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

#ifdef USE_LIBGO
    std::thread m{[&](){co_sched.Start(4);}};
    m.detach();
    go [&](){
        proxy(port, method, pass);
    };
#else
    std::async(proxy,port,method,pass);
#endif
    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}