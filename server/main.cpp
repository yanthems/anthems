

#include "test.hpp"
#include <iostream>
#include <cstdio>
int main() {
    
    auto raw = anthems::bytes(3);
    raw[0] = '1';
    raw[1] = '2';
    raw[2] = '3';

    auto res = anthems::cipher::md5Sum(raw);
    for (auto &i : res) {
        printf("%2.2X ", i);
    }
    std::getchar();
}