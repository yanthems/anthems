#include "bytes.hpp"

#include <cstring>
namespace anthems {

bytes::bytes(const std::string &str)
        : super(str.length()) {
    std::size_t index = 0;
    for (const auto &i : str) {
        this->at(index++) = i;
        //(*this)[index++] = i;
    }
}

bytes::bytes(const char *str)
        : super(std::strlen(str)) {
    for (unsigned int index = 0; *str != '\0'; str++) {
        this->at(index++) = *str;
    }
}

bytes::bytes(std::size_t t)
        : super(t) {
}


bytes &bytes::cover(const anthems::bytes &data, std::size_t begin) {
    for (const auto &i:data) {
        this->at(begin++) = i;
    }
    return *this;
}

bytes &bytes::operator+=(const bytes &data) {
    auto start = this->size();
    this->resize(start + data.size());
    for (auto i = 0; i < data.size(); i++) {
        this->at(start + i) = data[i];
    }
    return *this;
}

bytes &bytes::operator+=(const char *str) {
    auto start = this->size();
    int len = std::strlen(str);
    this->resize(start + len);
    for (auto i = 0; i < len; i++) {
        this->at(start + i) = *(str + i);
    }
    return *this;
}

bytes &bytes::operator+=(const std::string &str) {
    auto start = this->size();
    this->resize(start + str.size());
    for (auto i = 0; i < str.size(); i++) {
        this->at(start + i) = str[i];
    }
    return *this;
}

bytes operator+(const bytes &src, const bytes &data) {
    auto res = src;
    res += data;
    return res;
}

bytes operator+(const bytes &src, const char *str) {
    auto res = src;
    res += str;
    return res;
}

bytes operator+(const bytes &src, const std::string &str) {
    auto res = src;
    res += str;
    return res;
}

bytes bytes::split(std::size_t begin, std::size_t end) {
    if (end == ToEnd||end>this->size()) {
        end = this->size();
    }
    if(end<begin){
        return bytes();
    }
    auto res = bytes(end - begin);
    std::size_t index = 0;
    for (auto i = begin; i < end; i++) {
        res[index++] = this->at(i);
    }
    return res;
}

std::string bytes::to_string() {
//    return std::string((char*)this->data());
    std::string res;
    for(auto&i:*this){
        res+=i;
    }
    return res;
}

std::ostream &operator<<(std::ostream &out,const anthems::bytes &data) {
#ifdef HEX
    out << std::hex;
    for (const auto &i:data) {
        out << int(i);
    }
    out<<std::dec;
#else
    for(const auto&i:data){
        out<<i;
    }
#endif
    return out;
}

}
