

#include "test.hpp"
#include <iostream>
#include <cstdio>
int main() {
    
    auto raw=anthems::bytes(std::string("12"));
    raw += anthems::bytes("3");

    auto res = anthems::cipher::md5Sum(raw);
    for (auto&i : res) {
        std::printf("%2.2X", i);
    }
    std::getchar();
}