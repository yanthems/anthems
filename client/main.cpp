
#include "logger.hpp"
#include "ssc.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

#include "cipher.hpp"
#include "protocol.hpp"

#include "protocol.hpp"
#include "safe_queue.hpp"

#include <thread_pool.hpp>
#include <future>
#include <chrono>

int handle(anthems::ss_conn&& conn,const std::string&host,const std::string&port) {
    anthems::tcp_client client;
    auto cipher=anthems::cipher("aes-256-cfb","test23334");


    auto create_cipconn=[&](anthems::bytes&&helloData){
//        auto [shost,sport]=anthems::sockv5::parse_addr(helloData);
//        if(shost.empty()||sport.empty()){
//            throw std::logic_error("host or port is empty");
//        }
        auto mcip=cipher;
        auto cip_c=anthems::cipher_conn(client.connect(host,port),std::move(mcip));
        cip_c.write(helloData);
        return cip_c;
    };

    try {
        auto s5=anthems::sockv5(std::forward<anthems::ss_conn>(conn));
        auto cip_c=create_cipconn(s5.get_request());
        auto f1=std::async([&](){
            return anthems::ss_conn::pipe_then_close(s5.get(),cip_c,"local say:");
        });
        anthems::ss_conn::pipe_then_close(cip_c,s5.get(),"remote say:");
        anthems::log("local count=", f1.get());
        anthems::log("==try close cipher conn==");
        cip_c->close();
    }catch (const std::exception&e){
        anthems::log(e.what());
    }

    return 0;
}
void listen(const std::string&port) {

    anthems::tcp_server server(port, anthems::tcpv4);
    thread_pool tp(4);

    safe_queue<std::future<int>> res;


    try {
        std::thread t1([&]() {
            while (true) {
                auto conn = server.accept();
                anthems::log("push");
#if 0
                res.push(tp.add(handle,
                               std::move(conn),
                               "127.0.0.1",
                               "1088"));
#else
        handle(std::move(conn),"127.0.0.1","23334");
#endif
            }
        });
        while (true) {
            anthems::log("get");
            res.pop_front().wait_for(std::chrono::seconds(30));
        }
        t1.join();
    } catch (const std::exception &e) {
        anthems::log(e.what());
    }

}

int main(){

    listen("1088");
}