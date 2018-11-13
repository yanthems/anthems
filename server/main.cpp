

#include "cipher.hpp"
#include "ssconn.hpp"

#include "logger.hpp"
#include "sss.hpp"

int main() {

    auto s=anthems::ssserver("23334");


    s.run();
}