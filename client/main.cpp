
#include "logger.hpp"
#include "ssc.hpp"

#include "cipher_conn.hpp"

#include "cipher.hpp"

int main() {

    try {
        anthems::tcp_client client;

        auto c = client.connect("127.0.0.1", "23334");

        auto cc=anthems::cipher_conn(
                std::move(c),"aes-256-cfb","foobar");

        anthems::bytes raw(anthems::cipher_conn::Block);
        raw.cover("fuck gfw!");

        cc.write(raw);

        auto res=cc.read();

        anthems::log("server echo:",res);

    } catch (std::exception &e) {
        anthems::log(e.what());
    }


}