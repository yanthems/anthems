

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

void proxy(const std::string&port) {
    auto server = anthems::tcp_server(port, anthems::tcpv4);
    auto client = anthems::tcp_client();

    auto cipher = anthems::cipher("aes-256-cfb", "test23334");
    thread_pool tp(4);
    safe_queue<std::future<void>> res;
    try {
        while (true) {
            auto cip = cipher;
            auto cip_c = anthems::cipher_conn(server.accept(), std::move(cip));

//                res.push(tp.add([&](){
            try {
                auto addr = cip_c.read(anthems::sockv5::RequestSize);
                auto[host, port]=anthems::sockv5::parse_addr(addr);

                auto sp = anthems::simple(client.connect(host, port));

                auto f1 = std::async([&]() {
                    return anthems::ss_conn::pipe_then_close(cip_c, sp.get(), "local to server");
                });
                anthems::ss_conn::pipe_then_close(sp.get(), cip_c, "server to local ");

                anthems::log("local count=", f1.get());
                anthems::log("==try close cipher conn==");
                cip_c->close();
            } catch (const std::exception &e) {
                anthems::log(e.what());
                continue;
            }
//                }));
        }

    } catch (const std::exception &e) {
        anthems::log(e.what());
    }

}
int main() {

    proxy("23334");
}