

#include "cipher.hpp"
#include "ssconn.hpp"

#include <iostream>
#include <cstdio>
int main() {
    
    auto c = new anthems::cipher("aes-128-cfb","foobar");
    auto con = new anthems::ssconn;
    std::getchar();
}