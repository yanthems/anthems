

#include "cipher.hpp"
#include "ssconn.hpp"

#include "logger.hpp"
#include "sss.hpp"

int main() {

    auto s=anthems::ssserver("23334");

    try {
        s.run();
    } catch (const std::exception&e) {
        anthems::log(e.what());
    }
}