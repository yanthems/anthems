

#include "cipher.hpp"
#include "ss_conn.hpp"

#include "logger.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

#include <future>
#include <ssc.hpp>

void test(){
    auto s = anthems::tcp_server("23334",anthems::tcpv4);
    try {
        while (true) {
            auto c = s.accept();

            auto cc=anthems::cipher_conn(
                    std::move(c),"aes-256-cfb","foobar");

            auto res = cc.read();
            anthems::log("client say:",res);
            cc.write(res);
        }
    } catch (const std::exception &e) {
        anthems::log(e.what());
    }
}

void proxy(const std::string&port){
    auto server=anthems::tcp_server(port,anthems::tcpv4);
    auto client=anthems::tcp_client();
    try {
        auto cipher=anthems::cipher("aes-256-cfb","foobar");
        while (true){
            auto cip=cipher;
            auto cc=anthems::cipher_conn(server.accept(),std::move(cip));
            std::async([&](){

                //todo get request;
                anthems::ss_conn remote_c;
                try {
                    remote_c=client.connect("http://www.baidu.com");
                }catch (const std::exception&e){
                    anthems::log(e.what());
                    return;
                }

                auto f1=std::async([&](){
                    auto res=anthems::ss_conn::pip_then_close(cc,remote_c);
                    return res;
                });
                auto f2=std::async([&](){
                    auto res=anthems::ss_conn::pip_then_close(remote_c,
                    cc);
                    return res;
                });

                anthems::log("proxy count=",f1.get()+f2.get());
            }).get();
        }
    }catch (const std::exception&e){
        anthems::log(e.what());
    }

}
int main() {

    proxy("23334");
}