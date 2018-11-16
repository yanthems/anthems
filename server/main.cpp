

#include "cipher.hpp"
#include "ss_conn.hpp"

#include "logger.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

#include <future>
#include <ssc.hpp>
#include <helper.hpp>

#include <thread_pool.hpp>

void proxy(const std::string&port){
    auto server=anthems::tcp_server(port,anthems::tcpv4);
    auto client=anthems::tcp_client();
    try {
        auto cipher=anthems::cipher("aes-256-cfb","test23334");
        thread_pool tp(4);
        std::vector<std::future<void>>res;
        while (true){
            auto cip=cipher;
            auto cip_c=anthems::cipher_conn(server.accept(),std::move(cip));
            
            res.emplace_back(tp.add([&](){
                
                auto addr=cip_c.read(263);
                auto [host,port]=anthems::parse_addr(addr);
                
                anthems::ss_conn remote_c;
                try {
                    remote_c=client.connect(host,port);
                }catch (const std::exception&e){
                    anthems::log(e.what());
                    return;
                }
                

                auto f1=std::async([&](){
                    return anthems::ss_conn::pipe_then_close(cip_c, remote_c, "recv from local,send to server");
                });
                auto f2=std::async([&](){
                    return anthems::ss_conn::pipe_then_close(remote_c, cip_c, "recv from server,send to local ");
                });

                anthems::log("local count=",f1.get(),"sever count=",f2.get());
                remote_c->shutdown(remote_c->shutdown_both);
                cip_c->shutdown(cip_c->shutdown_both);
                anthems::log("==try close==");
                cip_c->close();
                remote_c->close();
            }));

         
        }
        for(auto &&i:res){
            i.get();
        }
    }catch (const std::exception&e){
        anthems::log(e.what());
    }

}
int main() {

    proxy("23334");
}