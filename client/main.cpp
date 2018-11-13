
#include "logger.hpp"
#include "ssc.hpp"

int main() {
    std::cout << "hi";

    try {
        anthems::ssclient client("127.0.0.1","23334");

        client.write("fuckyou");

        auto res=client.read();

        for(auto&i:res){
            std::cout<<i;
        }
    }catch (std::exception&e){
        anthems::log(e.what());
    }



}