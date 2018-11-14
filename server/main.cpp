

#include "cipher.hpp"
#include "ss_conn.hpp"

#include "logger.hpp"
#include "sss.hpp"
#include "cipher_conn.hpp"

int main() {

    auto s = anthems::tcp_server("23334");

    try {
        while (true) {
            auto c = s.accept();

            auto cc=anthems::cipher_conn(std::move(c),"aes-128-cfb","foobar");

            auto res = cc.read();

            cc.write(res);
        }
    } catch (const std::exception &e) {
        anthems::log(e.what());
    }
}